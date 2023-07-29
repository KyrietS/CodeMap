#include "pch.hpp"
#include "ImageEntity.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Canvas.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"


namespace 
{
	struct Script : public ScriptableEntity
	{
		void OnUpdate() override
		{
		}
	};
}

ImageEntity::ImageEntity(const Entity& entity, EventQueue& eventQueue)
	: Entity(entity)
{
	AddComponent<Components::Image>();
	AddComponent<Components::Focusable>();
	
	AttachScript<::Script>();
	AttachScript<MoveByDragScript>(std::ref(eventQueue));
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
