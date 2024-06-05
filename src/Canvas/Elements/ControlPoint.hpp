#pragma once
#include "Render/VColor.hpp"
#include "Events/Event.hpp"
#include "Events/EventQueue.hpp"
#include "Events/MouseEvents.hpp"
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

class CanvasCamera;

namespace Elements
{
	struct ControlPoint
	{
		ControlPoint(const glm::vec2& position, const CanvasCamera& camera, EventQueue& eventQueue)
			: Position(position), m_Camera(camera), m_EventQueue(eventQueue) {}

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
		void Draw(const CanvasCamera&) const;
		void OnEvent(Event&);
		bool Contains(const glm::vec2& point) const
		{
			float distance = glm::distance(Position, point);
			return distance <= Radius;
		}

	private:
		bool OnMousePressed(const Events::Input::MousePressed&);
		bool OnMouseReleased(const Events::Input::MouseReleased&);
		void OnUpdate(Event&);

		const CanvasCamera& m_Camera;
		EventQueue& m_EventQueue;
	};
}