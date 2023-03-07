#pragma once

#include "Canvas/Entity.hpp"


class ArrowEntity : public Entity
{
public:
	ArrowEntity(const Entity& entity);
	ArrowEntity& Build();
};

