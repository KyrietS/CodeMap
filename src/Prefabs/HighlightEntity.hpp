#pragma once

#include "Canvas/Entity.hpp"


class HighlightEntity : public Entity
{
public:
	HighlightEntity(const Entity& entity, EventQueue&);

	static constexpr float EDIT_POINT_RADIUS = 10.0f;
};

