#include "pch.hpp"
#include "ImageEntity.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Canvas.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"


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
			Renderer::UnloadImage(GetComponent<Components::Image>().TextureId);
		}

		ImageEntity m_Entity;
	};
}

ImageEntity::ImageEntity(const Entity& entity)
	: Entity(entity)
{
	AddComponent<Components::Image>();
	AddComponent<Components::Focusable>();
	
	AttachScript<::Script>(*this);
	AttachScript<MoveByDragScript>();
	AttachScript<CommonCanvasEntityScript>();
}

ImageEntity& ImageEntity::Build(glm::vec2 pos, uint8_t* data, int width, int height)
{
	size_t dataSize = width * height;
	std::span<uint8_t> pixels{ data, dataSize };
	auto& image = Image();
	image.TextureId = Renderer::LoadTextureFromBytes(pixels, width, height);
	image.Width = width;
	image.Height = height;

	Transform().Translation = pos;
	Focusable().Size = { (float)width, (float)height };
	return *this;
}
