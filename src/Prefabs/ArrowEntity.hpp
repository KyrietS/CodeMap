#pragma once

#include "Canvas/Entity.hpp"


class ArrowEntity : public Entity
{
public:
	ArrowEntity(const Entity& entity);
	ArrowEntity& Build();

	static constexpr float EDIT_POINT_RADIUS = 10.0f;
};

