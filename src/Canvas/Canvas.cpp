#include "pch.hpp"

#include "Canvas.hpp"
#include "raylib.h"
#include "raymath.h"
#include "clip.h"
#include "Components.hpp"
#include "Entity.hpp"
#include "Prefabs/ImageEntity.hpp"
#include "Prefabs/LineEntity.hpp"
#include "Utils/Print.hpp"
#include "Scripts/ClickToAddTextScript.hpp"
#include "Scripts/SelectionScript.hpp"
#include "Scripts/CanvasViewControlScript.hpp"
#include "Utils/Print.hpp"
#include "Input.hpp"


Canvas* Canvas::m_PrimaryInstance = nullptr;


Canvas::Canvas(bool primary) : m_Props{}
{
	// TODO: I'm not sure if this is good place for creating scripts...
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

void Canvas::Draw()
{
	BeginDrawing();
	ClearBackground(m_Props.BackgroundColor);
	
	// Canvas world drawing
	BeginMode2D(m_Camera);
	{
		DrawGrid();

		// TODO: Layer system so that GUI is never "under" canvas elements.
		m_Registry.sort<Components::Transform>([](const auto& lhs, const auto& rhs) {
			return lhs.Index < rhs.Index;
		}, entt::insertion_sort{}); // Insertion sort is O(n) for nearly sorted arrays

		// Draw entities in order of their z-Index
		auto drawables = m_Registry.view<Components::Transform>();
		for (Entity entity : drawables)
		{
			auto& transform = entity.GetComponent<Components::Transform>();

			// Draw sprite
			if (entity.HasComponent<Components::Sprite>())
			{
				auto& sprite = entity.GetComponent<Components::Sprite>();
				DrawTextureV(sprite, transform.Translation, WHITE);
			}

			// Draw line
			if (entity.HasComponent<Components::LineSegment>())
			{
				auto& line = entity.GetComponent<Components::LineSegment>();
				Vector2 begin = line.GetBegin(transform);
				Vector2 end = line.GetEnd(transform);
				DrawLineEx(begin, end, line.Thickness, line.StrokeColor);
			}

			// Draw arrowhead
			if (entity.HasComponent<Components::Arrowhead>())
			{
				auto& arrowhead = entity.GetComponent<Components::Arrowhead>();
				Vector2 tip1 = arrowhead.Offset;
				Vector2 tip2 = { tip1.x - arrowhead.Width, tip1.y - arrowhead.Height };
				Vector2 tip3 = { tip1.x - arrowhead.Width, tip1.y + arrowhead.Height };

				tip1 = Vector2Transform(tip1, transform.GetTransformMatrix());
				tip2 = Vector2Transform(tip2, transform.GetTransformMatrix());
				tip3 = Vector2Transform(tip3, transform.GetTransformMatrix());

				DrawTriangle(tip1, tip2, tip3, ORANGE);
			}

			// Draw text
			if (entity.HasComponent<Components::Text>())
			{
				auto& text = entity.GetComponent<Components::Text>();
				const auto& position = transform.Translation;
				Font font = GetFontDefault();
				float spacing = text.Size / 10.0f;
				DrawTextEx(font, text, position, text.Size, spacing, text.FontColor);
			}
		}

		// Draw selection rects
		auto viewFocus = m_Registry.view<Components::Transform, Components::Focusable>();
		for (auto [entity, transform, focusable] : viewFocus.each())
		{
			// TODO: This will not work with rotations:
			const Rectangle rect = focusable.AsRectangle(transform);

			if (focusable.IsFocused)
				DrawRectangleLinesEx(rect, 2.0f, BLUE);
			else if (m_DebugMode) // this is debug line. It should always be 1px thick
				DrawRectangleLinesEx(rect, 1.0f / m_Camera.GetZoom(), RED);
		}
		
	}
	EndMode2D();

	// TODO: Move GUI outside the Canvas class. Everything here should be rendered "inside" camera.
	//       Overlay with GUI should still have access to Canvas to manipulate it via provided API.
	//       (without friendship would be great)
	DrawGui();
	
	EndDrawing();
}

void Canvas::OnUpdate()
{
	// TODO: This code should go to NativeScript
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V))
	{
		HandlePasteImage();
	}
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		LineEntity(CreateEntity(Input::GetWorldMousePosition())).Build({0.0f, 0.0f});
	}
	if (IsKeyPressed(KEY_TAB))
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

Entity Canvas::CreateEntity(Vector2 initialPosition)
{
	static int s_LastIndex = 0;

	Entity entity = CreateVoidEntity();
	auto& transform = entity.AddComponent<Components::Transform>(initialPosition);
	transform.Index = s_LastIndex;

	s_LastIndex += 1;

	print("index = {}", s_LastIndex);

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
	print("CREATED ENTITY id={}", (int)(entt::entity)entity);

	return entity;
}

void Canvas::HandlePasteImage()
{
	clip::image clipboardImage;
	if (!clip::get_image(clipboardImage))
	{
		return TraceLog(LOG_WARNING, "CLIPBOARD: Failed to paste an image from clipboard");
	}

	clip::image_spec imageSpec = clipboardImage.spec();
	int size = (int)imageSpec.required_data_size();
	uint8_t* data = reinterpret_cast<uint8_t*>(clipboardImage.data());
	TransformFromBgraToRgba(data, size);
	Vector2 imagePos = Input::GetWorldMousePosition();

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
	const int NUM_OF_DOTS_HORIZONTAL = static_cast<int>((GetScreenWidth() / zoom) / DOT_GAP_SIZE + 2);
	const int NUM_OF_DOTS_VERTICAL = static_cast<int>((GetScreenHeight() / zoom) / DOT_GAP_SIZE + 2);
	float DOT_SIZE = 4;
	if (zoom < 1.0f)
	{
		DOT_SIZE *= (2.0f - zoom);
	}

	Vector2 dotScreenPos = { 0, 0 };
	Vector2 dotWorldPos = m_Camera.GetScreenToWorld(dotScreenPos);
	Vector2 firstDotScreenPos = {
		DOT_GAP_SIZE * ceil(dotWorldPos.x / DOT_GAP_SIZE) - DOT_GAP_SIZE,
		DOT_GAP_SIZE * ceil(dotWorldPos.y / DOT_GAP_SIZE) - DOT_GAP_SIZE
	};

	for (int i = 0; i < NUM_OF_DOTS_VERTICAL; i++)
	{
		float dotY = firstDotScreenPos.y + (i * DOT_GAP_SIZE);
		for (int k = 0; k < NUM_OF_DOTS_HORIZONTAL; k++)
		{
			float dotX = firstDotScreenPos.x + (k * DOT_GAP_SIZE);
			DrawRectangleV({ dotX, dotY }, { DOT_SIZE, DOT_SIZE }, LIGHTGRAY);
		}
	}
}

void Canvas::DrawGui()
{
	// Draw zoom level
	std::string zoomLevelText = "zoom: " + std::to_string(int(m_Camera.GetZoom() * 100)) + "%";
	DrawText(zoomLevelText.c_str(), 30, GetScreenHeight() - 30, 10, DARKGRAY);
}
