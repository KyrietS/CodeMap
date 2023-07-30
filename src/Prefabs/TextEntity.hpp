#pragma once

#include "Canvas/Entity.hpp"
#include "Events/EventQueue.hpp"


class TextEntity : public Entity
{
public:
	TextEntity(const Entity& entity, EventQueue&);
	TextEntity& Build(const std::string& text, float fontSize = 32.0f);
};
