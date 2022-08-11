#include "pch.hpp"
#include "ImageEntity.hpp"
#include "Components.hpp"
#include "Canvas.hpp"

namespace 
{
	struct Script : public ScriptableEntity
	{
		Script(ImageEntity entity)
			: m_Entity(entity)
		{
		}

		bool m_CurrentlyMoving = false;

		void OnUpdate() override
		{
			auto isFocused = GetComponent<Components::Focusable>().IsFocused;
			Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), Canvas::Camera());
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isFocused
				&& (m_CurrentlyMoving || CheckCollisionPointRec(worldPos, m_Entity.AsRectangle())))
			{
				m_CurrentlyMoving = true;
				// Scale screen distance to world distance
				Vector2 delta = Vector2Scale(GetMouseDelta(), 1.0f / Canvas::Camera().zoom);
				MoveBy(delta);
			}

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				m_CurrentlyMoving = false;
			}

			if (isFocused && IsKeyPressed(KEY_PAGE_UP))
			{
				GetComponent<Components::Transform>().Index += 1;
			}
			if (isFocused && IsKeyPressed(KEY_PAGE_DOWN))
			{
				GetComponent<Components::Transform>().Index -= 1;
			}
			if (isFocused && IsKeyPressed(KEY_DELETE))
			{
				Canvas::Get().RemoveEntity(m_Entity);
			}
		}

		void MoveBy(Vector2 positionChange)
		{
			Components::Transform& transform = m_Entity.GetComponent<Components::Transform>();
			transform.Translation = Vector2Add(transform, positionChange);

			Components::Focusable& focus = m_Entity.GetComponent<Components::Focusable>();
			focus.FocusArea = m_Entity.AsRectangle();
		}

		void OnDestroy() override
		{
			UnloadTexture(GetComponent<Components::Sprite>());
		}

		ImageEntity m_Entity;
	};
}

ImageEntity::ImageEntity()
	: Entity({ Canvas::Get().CreateEntity(), &Canvas::Get()})
{
	AddComponent<Components::Sprite>();
	AddComponent<Components::Focusable>();
	AddComponent<Components::NativeScript>().Bind<::Script>(*this);
}

ImageEntity::ImageEntity(Vector2 pos, uint8_t* data, int width, int height)
	: ImageEntity()
{
	Image image = LoadImageFromRgba(data, width, height);
	// TODO: Is it a good idea to unload the texture in script's OnDestroy?
	Texture().Texture = LoadTextureFromImage(image);
	Transform().Translation = pos;
	Focusable().FocusArea = AsRectangle();
	UnloadImage(image);
}

Rectangle ImageEntity::AsRectangle()
{
	Vector2 position = GetComponent<Components::Transform>();
	Texture2D texture = GetComponent<Components::Sprite>();
	return Rectangle{ position.x, position.y, (float)texture.width, (float)texture.height };
}

Image ImageEntity::LoadImageFromRgba(uint8_t* data, int width, int height)
{
	PixelFormat format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	int size = GetPixelDataSize(width, height, format);

	Image image{};
	image.data = RL_MALLOC(size);
	if (image.data == nullptr)
	{
		TraceLog(LOG_ERROR, "IMAGE: Cannot alocate memory for image");
		return image;
	}
	memcpy(image.data, data, size);
	image.width = width;
	image.height = height;
	image.mipmaps = 1;
	image.format = format;

	return image;
}
