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

	bool Contains(const Box& other)
	{
		return (other.x >= this->x)
			&& (other.x + other.width <= this->x + this->width)
			&& (other.y >= this->y)
			&& (other.y + other.height <= this->y + this->height);
	}

	static Box Create(const std::vector<glm::vec2>& points)
	{
		if (points.empty())
			return {};
		float minX = points[0].x;
		float minY = points[0].y;
		float maxX = points[0].x;
		float maxY = points[0].y;
		for (const auto& point : points)
		{
			minX = std::min(minX, point.x);
			minY = std::min(minY, point.y);
			maxX = std::max(maxX, point.x);
			maxY = std::max(maxY, point.y);
		}
		return {
			.x = minX,
			.y = minY,
			.width = maxX - minX,
			.height = maxY - minY
		};
	}
};

