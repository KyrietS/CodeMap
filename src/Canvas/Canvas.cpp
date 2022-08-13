#include "pch.hpp"

#include "Canvas.hpp"
#include "raylib.h"
#include "raymath.h"
#include "clip.h"
#include "Components.hpp"
#include "Entity.hpp"
#include "Prefabs/ImageEntity.hpp"
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
	CreateEntity().AddComponent<Components::NativeScript>().Bind<SelectionScript>();
	CreateEntity().AddComponent<Components::NativeScript>().Bind<CanvasViewControlScript>();
	CreateEntity().AddComponent<Components::NativeScript>().Bind<ClickToAddTextScript>();


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

		// Draw text
		{
			auto viewText = m_Registry.view<Components::Transform, Components::Text>();
			for (auto [entity, transform, text] : viewText.each())
			{
				const auto& position = transform.GetTransform();
				Font font = GetFontDefault();
				float spacing = text.Size / 10.0f;
				DrawTextEx(font, text, position, text.Size, spacing, text.FontColor);
			}
		}


		// Draw sprites (order_by TrasnformComponent)
		{
			auto viewTexture = m_Registry.view<Components::Transform, Components::Sprite>().use<Components::Transform>();
			for (auto [entity, transform, texture] : viewTexture.each())
			{
				DrawTextureV(texture, transform, WHITE);
			}
		}

		// Draw selection rects
		{
			auto viewFocus = m_Registry.view<Components::Focusable>();
			for (auto [entity, focusable] : viewFocus.each())
			{
				if (focusable.IsFocused)
					DrawRectangleLinesEx(focusable.FocusArea, 2.0f, BLUE);
				else
					DrawRectangleLinesEx(focusable.FocusArea, 1.0f, RED);
			}
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
	// Move it to some kinf ScriptEngine.
	// Waste of time to check this if every frame.
	//auto view = m_Registry.view<Components::NativeScript>();
	//for (auto [entity, script] : view.each())
	//{
	//	if (!script.Instance)
	//	{
	//		script.Instance = script.Instantiate();
	//		print("Instantiating script: {}", typeid(*(script.Instance)).name());
	//		script.Instance->m_Entity = { entity, this };
	//		script.Instance->OnCreate();
	//	}
	//	script.Instance->OnUpdate();
	//}

	// TODO: This code should go to NativeScript
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V))
	{
		HandlePasteImage();
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
	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<Components::Transform>(initialPosition);
	print("CREATED ENTITY id={}", (int)(entt::entity)entity);

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
		m_Registry.get<Components::NativeScript>(entity).Instance->OnDestroy();
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
