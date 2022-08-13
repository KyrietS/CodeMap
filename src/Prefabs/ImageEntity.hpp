#pragma once

#include "Canvas/ScriptableEntity.hpp"
#include "entt.hpp"
#include "Canvas/Components.hpp"


class ImageEntity : public Entity
{
public:
	ImageEntity(const Entity& entity);
	ImageEntity& Build(Vector2 pos, uint8_t* data, int width, int height);

	Rectangle AsRectangle();

private:
	Image LoadImageFromRgba(uint8_t* data, int width, int height);

private:
	Components::Transform& Transform() { return GetComponent<Components::Transform>(); }
	Components::Sprite& Texture() { return GetComponent<Components::Sprite>(); }
	Components::Focusable& Focusable() { return GetComponent<Components::Focusable>(); }
};

