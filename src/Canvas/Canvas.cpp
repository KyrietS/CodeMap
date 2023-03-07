#include "pch.hpp"

#include "Canvas.hpp"
#include "clip.h"
#include "Components.hpp"
#include "Entity.hpp"
#include "Prefabs/ImageEntity.hpp"
#include "Prefabs/ArrowEntity.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Window.hpp"
#include "Controllers/CameraController.hpp"
#include "Controllers/SelectionController.hpp"
#include "Controllers/TextController.hpp"
#include "Controllers/ImageController.hpp"
#include "Controllers/LineController.hpp"


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


Canvas::Canvas(bool primary) : m_Props{}
{
	m_Controllers.push_back(std::make_unique<CameraController>(m_Camera));
	m_Controllers.push_back(std::make_unique<SelectionController>());
	m_Controllers.push_back(std::make_unique<TextController>());
	m_Controllers.push_back(std::make_unique<ImageController>());
	m_Controllers.push_back(std::make_unique<LineController>());

	if (primary)
		m_PrimaryInstance = this;
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
				Renderer::DrawLine(begin, end, arrow.Thickness, arrow.StrokeColor);

				// arrowhead directed to the right and pointing at (0,0)
				float arrowheadSize = 20.0f;
				glm::vec2 tip1{ -30.0f, -10.0f };
				glm::vec2 tip2{ -30.0f, +10.0f };
				glm::vec2 tip3{ 0.0f, 0.0f };

				// rotate relative to (0,0)
				float angle = arrow.GetAngle();
				tip1 = glm::rotate(tip1, angle);
				tip2 = glm::rotate(tip2, angle);
				tip3 = glm::rotate(tip3, angle); // redundant rotation of (0,0)

				// translate
				tip1 += arrow.GetEnd(transform);
				tip2 += arrow.GetEnd(transform);
				tip3 += arrow.GetEnd(transform);

				Renderer::DrawTriangle(tip1, tip2, tip3, VColor::Orange);
			}

			// Draw text
			if (entity.HasComponent<Components::Text>())
			{
				const auto& text = entity.GetComponent<Components::Text>();
				const auto& position = transform.Translation;
				Renderer::DrawText(position, text);
			}
		}

		// Draw selection rects
		auto viewFocus = m_Registry.view<Components::Transform, Components::Focusable>();
		for (auto [entity, transform, focusable] : viewFocus.each())
		{
			Box box = focusable.AsBox(transform);
			if (focusable.IsFocused)
				Renderer::DrawRectangleLines(box.GetPosition(), box.width, box.height, 2.0f, VColor::Blue);
			else if (m_DebugMode) // this is debug line. It should always be 1px thick
				Renderer::DrawRectangleLines(box.GetPosition(), box.width, box.height, 1.0f / m_Camera.GetZoom(), VColor::Red);
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

	// Remove entities scheduled for removal from scripts.
	for (auto entity : m_ToBeRemoved)
	{
		if (m_Registry.valid(entity))
			m_Registry.destroy(entity);
	}
	m_ToBeRemoved.clear();
}

void Canvas::OnEvent(Event& event)
{
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

Entity Canvas::CreateVoidEntity()
{
	Entity entity = { m_Registry.create(), this };
	LOG_DEBUG("Created entity id={}", (int)(entt::entity)entity);

	return entity;
}

void Canvas::ScheduleEntityForDestruction(const entt::entity entity)
{
	assert(m_Registry.valid(entity));
	if (m_Registry.all_of<Components::NativeScript>(entity))
	{
		auto& scripts = m_Registry.get<Components::NativeScript>(entity);
		for (auto& script : scripts.Instances)
		{
			if (script)
				script->OnDestroy();
		}
	}

	// Schedule the removal so the script can remove itself.
	m_ToBeRemoved.push_back(entity);
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
