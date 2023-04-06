#pragma once

#include "Canvas/ScriptableEntity.hpp"
#include "Canvas/Components.hpp"
#include <glm/vec2.hpp>


class ImageEntity : public Entity
{
public:
	ImageEntity(const Entity& entity);
	ImageEntity& Build(glm::vec2 pos, uint8_t* data, int width, int height);

private:
	Components::Transform& Transform() { return GetComponent<Components::Transform>(); }
	Components::Image& Image() { return GetComponent<Components::Image>(); }
	Components::Focusable& Focusable() { return GetComponent<Components::Focusable>(); }
};

