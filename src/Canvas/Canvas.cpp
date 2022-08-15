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

		// Draw arrows
		{
			auto viewLines = m_Registry.view<Components::Transform, Components::Arrow>().use<Components::Transform>();
			for (auto [entity, transform, arrow] : viewLines.each())
			{
				Vector2 transform = m_Registry.get<Components::Transform>(entity).GetTransform();
				Vector2 begin = Vector2Add(transform, arrow.Origin);
				Vector2 end = Vector2Add(begin, arrow.End);
				DrawLineEx(begin, end, arrow.Thickness, arrow.FillColor);

				
				if (arrow.EndHead != Components::Arrow::None)
				{
					// TODO: This arrowhead should be a separate component!
					float angle = Vector2Angle({ 1.0f, 0.0f }, arrow.End);
					float distance = Vector2Distance(begin, end);
					const int ARROW_SIZE = 40;
					Vector2 tip{ distance, 0.0f };
					Vector2 tip2{ distance - ARROW_SIZE, -ARROW_SIZE / 2.0f };
					Vector2 tip3{ distance - ARROW_SIZE, +ARROW_SIZE / 2.0f };

					Vector2 rotatedTip = Vector2Add(begin, Vector2Rotate(tip, angle));
					Vector2 rotatedTip2 = Vector2Add(begin, Vector2Rotate(tip2, angle));
					Vector2 rotatedTip3 = Vector2Add(begin, Vector2Rotate(tip3, angle));

					DrawTriangle(rotatedTip, rotatedTip2, rotatedTip3, ORANGE);
					//DrawCircleV(rotatedTip, 15.0f, ORANGE);

					//DrawTriangle()
				}
				
			}
		}

		// Draw selection rects
		if (m_DebugMode)
		{
			auto viewFocus = m_Registry.view<Components::Transform, Components::Focusable>();
			for (auto [entity, transform, focusable] : viewFocus.each())
			{
				// TODO: This will not work with rotations:
				const Rectangle rect = focusable.AsRectangle(transform);

				if (focusable.IsFocused)
					DrawRectangleLinesEx(rect, 2.0f, BLUE);
				else // this is debug line. It should always be 1px thick
					DrawRectangleLinesEx(rect, 1.0f / m_Camera.GetZoom(), RED);
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
