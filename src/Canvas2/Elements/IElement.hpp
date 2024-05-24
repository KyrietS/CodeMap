#pragma once
#include <glm/glm.hpp>

class Event;
struct Box;

namespace Elements
{
	class IElement
	{
	public:
		virtual ~IElement() = default;

		virtual void Draw() {};
		virtual void OnEvent(Event&) {};

		virtual void MoveBy(float x, float y) = 0;
		virtual bool Contains(const glm::vec2& point) const = 0;
		virtual Box GetBoundingBox() const = 0;
	};
}