#pragma once

#include "Canvas/Entity.hpp"


class TextEntity : public Entity
{
public:
	TextEntity(const Entity& entity);
	TextEntity& Build(const std::string_view text, float fontSize = 20.0f);
};
