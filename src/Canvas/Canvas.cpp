#include "pch.hpp"

#include "Canvas.hpp"
#include "clip.h"
#include "Components.hpp"
#include "Entity.hpp"
#include "Prefabs/ImageEntity.hpp"
#include "Prefabs/ArrowEntity.hpp"
#include "Prefabs/HighlightEntity.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Window.hpp"
#include "Controllers/CameraController.hpp"
#include "Controllers/SelectionController.hpp"
#include "Controllers/CommonCanvasEntityController.hpp"
#include "Controllers/CommonKeyboardShortcutsController.hpp"
#include "Controllers/TextController.hpp"
#include "Controllers/ImageController.hpp"
#include "Controllers/LineController.hpp"
#include "Controllers/ToolController.hpp"
#include "Controllers/HighlightController.hpp"
#include "Controllers/UndoRedoController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Serializer/SVG/SvgSerializer.hpp"
#include "Deserializer/SVG/SvgDeserializer.hpp"
#include "Events/GuiEvents.hpp"

namespace
{
	bool CompareTransforms(const Components::Transform& lhs, const Components::Transform& rhs)
	{
		size_t currIndex = 0;
		size_t end = std::min(lhs.IndexHierarchy.size(), rhs.IndexHierarchy.size());
		while (currIndex < end)
		{
			if (lhs.IndexHierarchy[currIndex] != rhs.IndexHierarchy[currIndex])
			{
				return lhs.IndexHierarchy[currIndex] < rhs.IndexHierarchy[currIndex];
			}
			currIndex += 1;
		}

		return lhs.IndexHierarchy.size() < rhs.IndexHierarchy.size();
	}
}


Canvas* Canvas::m_PrimaryInstance = nullptr;


Canvas::Canvas(EventQueue& eventQueue, bool primary) : m_Props{}, m_EventQueue(eventQueue)
{
	m_Controllers.push_back(std::make_unique<CameraController>(m_Camera));
	m_Controllers.push_back(std::make_unique<CommonCanvasEntityController>(m_EventQueue));
	m_Controllers.push_back(std::make_unique<ImageController>(m_EventQueue));
	m_Controllers.push_back(std::make_unique<ToolController>(m_EventQueue, m_Camera));
	m_Controllers.push_back(std::make_unique<UndoRedoController>(m_EventQueue, m_Registry));
	m_Controllers.push_back(std::make_unique<CommonKeyboardShortcutsController>(m_EventQueue));

	if (primary)
		m_PrimaryInstance = this;

	Animation::Init(m_EventQueue);
}

Canvas::~Canvas()
{
	m_PrimaryInstance = nullptr;
}

void Canvas::Draw()
{
	Renderer::ClearScreen(m_Props.BackgroundColor);
	
	// Canvas world drawing
	Renderer::BeginCameraView(m_Camera.GetData());
	{
		DrawGrid();
		// Transform hierarchy update
		UpdateHierarchy();
		// Insertion sort is O(n) for nearly sorted arrays
		m_Registry.sort<Components::Transform>(CompareTransforms, entt::insertion_sort{});

		// Draw entities in order of their z-Index
		auto drawables = m_Registry.view<Components::Transform>();
		for (Entity entity : drawables)
		{
			auto& transform = entity.GetComponent<Components::Transform>();

			// Draw sprite
			if (entity.HasComponent<Components::Image>())
			{
				auto& sprite = entity.GetComponent<Components::Image>();
				Renderer::DrawImage(transform.Translation, sprite);
			}

			// Draw arrow
			if (entity.HasComponent<Components::Arrow>())
			{
				auto& arrow = entity.GetComponent<Components::Arrow>();
				glm::vec2 begin = arrow.GetBegin(transform);
				glm::vec2 end = arrow.GetEnd(transform);
				glm::vec2 controlPoint = arrow.GetControlPoint(transform);

				Renderer::DrawBezier(begin, controlPoint, end, arrow.Thickness, arrow.StrokeColor);

				// arrowhead directed to the right and pointing at (0,0)
				float arrowheadSize = 30.0f * arrow.Thickness / 5;
				glm::vec2 tip1{ -arrowheadSize, -arrowheadSize / 3 };
				glm::vec2 tip2{ -arrowheadSize, +arrowheadSize / 3 };
				glm::vec2 tip3{ 0.0f, 0.0f };

				// rotate relative to bezier control point
				float angle = arrow.GetEndAngle();
				tip1 = glm::rotate(tip1, angle);
				tip2 = glm::rotate(tip2, angle);
				tip3 = glm::rotate(tip3, angle); // redundant rotation of (0,0)

				// translate
				tip1 += end;
				tip2 += end;
				tip3 += end;

				Renderer::DrawTriangle(tip1, tip2, tip3, arrow.ArrowheadColor);

				// Draw edit points
				if (entity.GetComponent<Components::Focusable>().IsFocused)
				{
					float renderRadius = ArrowEntity::EDIT_POINT_RADIUS / 2;
					float radius = renderRadius / m_Camera.GetZoom();
					float thickness = 1.0f / m_Camera.GetZoom();

					// middle point
					Renderer::DrawCircle(controlPoint, radius, VColor::White);
					Renderer::DrawCircleOutline(controlPoint, radius, VColor::Blue);
					Renderer::DrawLine(begin, controlPoint, thickness, VColor::Blue);
					Renderer::DrawLine(controlPoint, end, thickness, VColor::Blue);

					Renderer::DrawCircle(begin, radius, VColor::White);
					Renderer::DrawCircleOutline(begin, radius, VColor::Blue);
					Renderer::DrawCircle(end, radius, VColor::White);
					Renderer::DrawCircleOutline(end, radius, VColor::Blue);
				}
			}

			// Draw highlight
			if (entity.HasComponent<Components::Highlight>())
			{
				const auto& highlight = entity.GetComponent<Components::Highlight>();
				if (highlight.Points.size() < 2)
					continue;

				auto points = highlight.GetWorldPoints(transform);
				Renderer::BeginBlendMode(highlight.BlendMode);
				Renderer::DrawPolygon(points, highlight.Color);
				Renderer::EndBlendMode();

				// Draw edit points
				if (entity.GetComponent<Components::Focusable>().IsFocused)
				{
					for (size_t i = 0; i < points.size(); i++)
					{
						glm::vec4 color = i == highlight.SelectedPointIndex ? VColor::Blue : VColor::White;
						float renderRadius = HighlightEntity::EDIT_POINT_RADIUS / 2;
						float radius = renderRadius / m_Camera.GetZoom();
						float thickness = 1.0f / m_Camera.GetZoom();
						Renderer::DrawCircle(points[i], radius, color);
						Renderer::DrawCircleOutline(points[i], radius, VColor::Blue);
					}
				}
			}

			// Draw text
			if (entity.HasComponent<Components::Text>())
			{
				const auto& text = entity.GetComponent<Components::Text>();
				const auto& position = transform.Translation;
				Renderer::DrawText(position, text);

				// Text bounding box
				if (entity.GetComponent<Components::Focusable>().IsFocused)
				{
					auto textDimensions = Renderer::MeasureText(text);
					glm::vec2 topLeftCorner = {position.x + textDimensions.Offset.x, position.y + textDimensions.Offset.y};
					float thickness = 1.0f / m_Camera.GetZoom();
					Renderer::DrawRectangleLines(topLeftCorner, textDimensions.Size.x, textDimensions.Size.y, thickness, VColor::LightGray);
				}
			}

		}

		// Draw selection rects
		auto viewFocus = m_Registry.view<Components::Transform, Components::Focusable>();
		for (auto [entity, transform, focusable] : viewFocus.each())
		{
			float thickness = 1.0f / m_Camera.GetZoom();
			Box box = focusable.AsBox(transform);
			if (focusable.IsFocused)
				Renderer::DrawRectangleLines(box.GetPosition(), box.width, box.height, thickness, focusable.BorderColor);
			else if (m_DebugMode) // this is debug line. It should always be 1px thick
				Renderer::DrawRectangleLines(box.GetPosition(), box.width, box.height, thickness, VColor::Red);
		}
		
		
	}
	Renderer::EndCameraView();
}

void Canvas::OnUpdate()
{
	for (const auto& controller : m_Controllers)
	{
		controller->OnUpdate();
	}

	if (Input::IsKeyPressed(Key::Tab))
	{
		m_DebugMode = !m_DebugMode;
	}

	if (Input::IsKeyPressed(Key::W))
	{
		m_EventQueue.Push(Events::Gui::ShowPopup{});
	}
}

void Canvas::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Canvas::Destroy>(BIND_EVENT(Canvas::OnDestroy));
	dispatcher.Dispatch<Events::Canvas::SaveToFile>(BIND_EVENT(Canvas::OnCanvasSaveToFile));
	dispatcher.Dispatch<Events::Canvas::LoadFromFile>(BIND_EVENT(Canvas::OnCanvasLoadFromFile));

	for (const auto& controller : m_Controllers)
	{
		controller->OnEvent(event);
	}
}

Entity Canvas::CreateEntity(glm::vec2 initialPosition)
{
	static int s_LastIndex = 0;

	Entity entity = CreateVoidEntity();
	entity.AddComponent<Components::Hierarchy>();
	auto& transform = entity.AddComponent<Components::Transform>(initialPosition);
	transform.Index = s_LastIndex;

	s_LastIndex += 1;

	return entity;
}

void Canvas::OnDestroy(const Events::Canvas::Destroy& event)
{
	entt::entity entity = entt::entity{ event.EntityId };
	if (!m_Registry.valid(entity))
	{
		return;
	}

	if (m_Registry.all_of<Components::NativeScript>(entity))
	{
		auto& scripts = m_Registry.get<Components::NativeScript>(entity);
		for (auto& script : scripts.Instances)
		{
			if (script)
				script->OnDestroy();
		}
	}

	m_Registry.destroy(entity);
	LOG_DEBUG("Entity {} destroyed", (int)entity);
	
}

void Canvas::OnCanvasSaveToFile(const Events::Canvas::SaveToFile& event)
{
	LOG_DEBUG("[EVENT] Canvas received SaveToFile event with path: {}", event.Filename);
	SvgSerializer{m_Registry}.Serialize();
}

void Canvas::OnCanvasLoadFromFile(const Events::Canvas::LoadFromFile& event)
{
	LOG_DEBUG("[EVENT] Canvas received LoadFromFile event with path: {}", event.Filename);
	SvgDeserializer{*this, m_Registry, m_EventQueue}.Deserialize(event.Filename);
}

Entity Canvas::CreateVoidEntity()
{
	Entity entity = { m_Registry.create(), *this };
	LOG_DEBUG("Created entity id={}", (int)(entt::entity)entity);

	return entity;
}

void Canvas::UpdateChildrenOf(Entity parent)
{
	if (!parent.HasComponent<Components::Hierarchy>())
		return;

	auto& children = parent.GetComponent<Components::Hierarchy>().Children;
	for (Entity child : children)
	{
		auto parentTransform = parent.HasComponent<Components::Transform>()
			? std::make_optional(std::ref(parent.GetComponent<Components::Transform>()))
			: std::nullopt;
		if (child.HasComponent<Components::Transform>())
		{
			child.GetComponent<Components::Transform>().Update(parentTransform);
		}
		UpdateChildrenOf(child);
	}
}

void Canvas::UpdateHierarchy()
{
	auto view = m_Registry.view<Components::Transform, Components::Hierarchy>();
	for (auto&& [entity, transform, hierarchy] : view.each())
	{
		bool isRootEntity = (hierarchy.Parent == entt::null);
		if (isRootEntity)
		{
			transform.Update(std::nullopt);
			UpdateChildrenOf(entity);
		}
	}
}

void Canvas::DrawGrid()
{
	const float zoom = m_Camera.GetZoom();
	const int DOT_GAP_SIZE = 50;
	const int NUM_OF_DOTS_HORIZONTAL = static_cast<int>((Window::GetWidth() / zoom) / DOT_GAP_SIZE + 2);
	const int NUM_OF_DOTS_VERTICAL = static_cast<int>((Window::GetHeight() / zoom) / DOT_GAP_SIZE + 2);
	float DOT_SIZE = 4;
	if (zoom < 1.0f)
	{
		DOT_SIZE *= (2.0f - zoom);
	}

	glm::vec2 dotScreenPos = { 0, 0 };
	glm::vec2 dotWorldPos = m_Camera.GetScreenToWorld(dotScreenPos);
	glm::vec2 firstDotScreenPos = {
		DOT_GAP_SIZE * ceil(dotWorldPos.x / DOT_GAP_SIZE) - DOT_GAP_SIZE,
		DOT_GAP_SIZE * ceil(dotWorldPos.y / DOT_GAP_SIZE) - DOT_GAP_SIZE
	};

	for (int i = 0; i < NUM_OF_DOTS_VERTICAL; i++)
	{
		float dotY = firstDotScreenPos.y + (i * DOT_GAP_SIZE);
		for (int k = 0; k < NUM_OF_DOTS_HORIZONTAL; k++)
		{
			float dotX = firstDotScreenPos.x + (k * DOT_GAP_SIZE);
			Renderer::DrawRectangle({ dotX, dotY }, DOT_SIZE, DOT_SIZE, VColor::LightGray);
		}
	}
}
