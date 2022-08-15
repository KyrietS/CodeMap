#pragma once

#include "Canvas/Entity.hpp"


class LineEntity : public Entity
{
public:
	LineEntity(const Entity& entity);
	LineEntity& Build(Vector2 end);
};

