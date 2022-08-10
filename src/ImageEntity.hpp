#pragma once

#include "ScriptableEntity.hpp"
#include "entt.hpp"
#include "Components.hpp"


class ImageEntity : public Entity
{
public:
	ImageEntity();
	ImageEntity(Vector2 pos, uint8_t* data, int width, int height);

	Rectangle AsRectangle();

private:
	Image LoadImageFromRgba(uint8_t* data, int width, int height);

private:
	Components::Transform& Transform() { return GetComponent<Components::Transform>(); }
	Components::Sprite& Texture() { return GetComponent<Components::Sprite>(); }
	Components::Focusable& Focusable() { return GetComponent<Components::Focusable>(); }
};

