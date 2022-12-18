#include "pch.hpp"

#include "Canvas.hpp"
#include "clip.h"
#include "Components.hpp"
#include "Entity.hpp"
#include "Prefabs/ImageEntity.hpp"
#include "Prefabs/LineEntity.hpp"
#include "Scripts/ClickToAddTextScript.hpp"
#include "Scripts/SelectionScript.hpp"
#include "Scripts/CanvasViewControlScript.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Window.hpp"


Canvas* Canvas::m_PrimaryInstance = nullptr;


Canvas::Canvas(bool primary) : m_Props{}
{
	// TODO: These should be implemented as controllers
	CreateVoidEntity().AddComponent<Components::NativeScript>().Bind<SelectionScript>();
	CreateVoidEntity().AddComponent<Components::NativeScript>().Bind<CanvasViewControlScript>();
	CreateVoidEntity().AddComponent<Components::NativeScript>().Bind<ClickToAddTextScript>();


	if (primary)
		m_PrimaryInstance = this;
}

Canvas::~Canvas()
{
	m_PrimaryInstance = nullptr;
}

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

// TODO: Move to Canvas class
void UpdateChildrenRecursively(Entity parent)
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
		UpdateChildrenRecursively(child);
	}
}

void UpdateHierarchy(entt::registry& registry)
{
	auto view = registry.view<Components::Transform, Components::Hierarchy>();
	for (auto &&[entity, transform, hierarchy]: view.each())
	{
		bool isRootEntity = (hierarchy.Parent == entt::null);
		if (isRootEntity)
		{
			transform.Update(std::nullopt);
			UpdateChildrenRecursively(entity);
		}
	}
}

void Canvas::Draw()
{
	Renderer::BeginFrame();
	Renderer::ClearScreen(m_Props.BackgroundColor);
	
	// Canvas world drawing
	Renderer::BeginCameraView(m_Camera.GetData());
	{
		DrawGrid();
		// Transform hierarchy update
		UpdateHierarchy(m_Registry);
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

			// Draw line
			if (entity.HasComponent<Components::LineSegment>())
			{
				auto& line = entity.GetComponent<Components::LineSegment>();
				glm::vec2 begin = line.GetBegin(transform);
				glm::vec2 end = line.GetEnd(transform);
				Renderer::DrawLine(begin, end, line.Thickness, line.StrokeColor);
			}

			// Draw arrowhead
			if (entity.HasComponent<Components::Arrowhead>())
			{
				auto& arrowhead = entity.GetComponent<Components::Arrowhead>();
				glm::vec2 tip1 = arrowhead.Offset;
				glm::vec2 tip2 = { tip1.x - arrowhead.Width, tip1.y - arrowhead.Height };
				glm::vec2 tip3 = { tip1.x - arrowhead.Width, tip1.y + arrowhead.Height };

				tip1 = transform.GetTransform() * glm::vec4{ tip1.x, tip1.y, 0.0f, 1.0f };
				tip2 = transform.GetTransform() * glm::vec4{ tip2.x, tip2.y, 0.0f, 1.0f };
				tip3 = transform.GetTransform() * glm::vec4{ tip3.x, tip3.y, 0.0f, 1.0f };

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

	// TODO: Move to a separate layer
	DrawGui();
	
	Renderer::EndFrame();
}

void Canvas::OnUpdate()
{
	// TODO: Move to a controller with event dispatcher
	if (Input::IsKeyDown(Key::LeftControl) && Input::IsKeyPressed(Key::V))
	{
		HandlePasteImage();
	}
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		LineEntity(CreateEntity(Input::GetWorldMousePosition())).Build();
		LOG_DEBUG("Created LineEntity");
	}
	if (Input::IsKeyPressed(Key::Tab))
	{
		m_DebugMode = !m_DebugMode;
	}
	if (Input::IsKeyPressed(Key::D))
	{
		LOG_DEBUG("Debug button pressed");
		Entity parent = CreateEntity(Input::GetWorldMousePosition());

		Entity child = CreateEntity();
		auto &ah = child.AddComponent<Components::Arrowhead>();
		ah.Height = 50.0f;
		ah.Width = 50.0f;

		parent.AddChild(child);
	}

	// Remove entities scheduled for removal from scripts.
	for (auto entity : m_ToBeRemoved)
	{
		if (m_Registry.valid(entity))
			m_Registry.destroy(entity);
	}
	m_ToBeRemoved.clear();
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

void TransformFromBgraToRgba(uint8_t* data, int size)
{
	for (int i = 0; i < size / 4; i++)
	{
		// Swap first and third byte in pixel.
		std::swap(data[i * 4 + 0], data[i * 4 + 2]);
	}
}

Entity Canvas::CreateVoidEntity()
{
	Entity entity = { m_Registry.create(), this };
	LOG_DEBUG("Created entity id={}", (int)(entt::entity)entity);

	return entity;
}

void Canvas::HandlePasteImage()
{
	clip::image clipboardImage;
	if (!clip::get_image(clipboardImage))
	{
		return LOG_WARN("CLIPBOARD: Failed to paste an image from clipboard");
	}

	clip::image_spec imageSpec = clipboardImage.spec();
	int size = (int)imageSpec.required_data_size();
	uint8_t* data = reinterpret_cast<uint8_t*>(clipboardImage.data());
	TransformFromBgraToRgba(data, size);
	glm::vec2 imagePos = Input::GetWorldMousePosition();

	ImageEntity(CreateEntity()).Build(imagePos, data, imageSpec.width, imageSpec.height);
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

void Canvas::DrawGui()
{
	std::string zoomLevelText = "zoom: " + std::to_string(int(m_Camera.GetZoom() * 100)) + "%";
	Renderer::DrawText({ 30, Window::GetHeight() - 30}, zoomLevelText, 10, VColor::DarkGray);

	std::string fps = std::to_string((int)Time::GetFPS()) + " FPS";
	Renderer::DrawText({ 30, Window::GetHeight() - 43}, fps, 10, VColor::DarkGray);
}
