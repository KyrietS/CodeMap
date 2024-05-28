#pragma once
#include "Render/VColor.hpp"

namespace Elements
{
	struct ControlPoint
	{
		// Properties
		glm::vec2 Position = { 0.0f, 0.0f };
		glm::vec4 BackgroundColor = VColor::White;
		glm::vec4 BorderColor = VColor::Blue;
		float Radius = 10.0f;
		float Thickness = 2.0f;

		// State
		bool Hovering = false;
		bool Dragging = false;

		// Methods
		bool Contains(const glm::vec2& point) const
		{
			float distance = glm::distance(Position, point);
			return distance <= Radius;
		}
	};
}