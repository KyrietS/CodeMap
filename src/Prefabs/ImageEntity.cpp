#include "pch.hpp"
#include "ImageEntity.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Canvas.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"
#include "Input.hpp"


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
		}

		void OnDestroy() override
		{
			UnloadTexture(GetComponent<Components::Sprite>());
		}

		ImageEntity m_Entity;
	};
}

ImageEntity::ImageEntity(const Entity& entity)
	: Entity(entity)
{
	AddComponent<Components::Sprite>();
	AddComponent<Components::Focusable>();
	
	AttachScript<::Script>(*this);
	AttachScript<MoveByDragScript>();
	AttachScript<CommonCanvasEntityScript>();
}

ImageEntity& ImageEntity::Build(Vector2 pos, uint8_t* data, int width, int height)
{
	Image image = LoadImageFromRgba(data, width, height);
	// TODO: Is it a good idea to unload the texture in script's OnDestroy?
	Texture().Texture = LoadTextureFromImage(image);
	Transform().Translation = pos;
	Focusable().Size = { (float)image.width, (float)image.height };
	UnloadImage(image);
	return *this;
}

Rectangle ImageEntity::AsRectangle()
{
	Vector2 position = GetComponent<Components::Transform>().Translation;
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
