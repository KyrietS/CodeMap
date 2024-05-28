#pragma once
#include <glm/glm.hpp>
#include "Canvas/Box.hpp"

class Event;

namespace Elements
{
	class IElement
	{
	public:
		virtual ~IElement() = default;

		virtual void Draw() {};
		virtual void OnEvent(Event&) {};

		virtual void MoveBy(float x, float y) = 0;
		virtual Box GetBoundingBox() const = 0;
		virtual bool Contains(const glm::vec2& point) const
		{
			return GetBoundingBox().Contains(point);
		}

		bool InEditMode = false;
	};
}