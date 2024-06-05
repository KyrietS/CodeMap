#pragma once
#include <glm/vec2.hpp>

struct Box
{
	float x;
	float y;
	float width;
	float height;

	glm::vec2 GetPosition()
	{
		return { x, y };
	}

	glm::vec2 GetSize()
	{
		return { width, height };
	}

	bool Contains(glm::vec2 point)
	{
		return (point.x >= this->x)
			&& (point.x <= (this->x + this->width))
			&& (point.y >= this->y)
			&& (point.y <= (this->y + this->height));
	}
};

