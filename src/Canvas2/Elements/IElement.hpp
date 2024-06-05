#pragma once
#include <glm/glm.hpp>
#include "Canvas2/Box.hpp"

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

		template<typename T>
		T* As()
		{
			return dynamic_cast<T*>(this);
		}

		template<typename T>
		const T* As() const
		{
			return dynamic_cast<const T*>(this);
		}

		// TODO: Consider adding: std::unique_ptr<IElement> Clone() const = 0;

		bool InEditMode = false;
	};
}