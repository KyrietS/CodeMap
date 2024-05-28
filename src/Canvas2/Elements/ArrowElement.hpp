#pragma once
#include "IElement.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "Events/MouseEvents.hpp"

class CanvasCamera;

namespace Elements
{
	class ArrowElement : public IElement
	{
	public:
		struct ControlPoint
		{
			glm::vec4 BackgroundColor = VColor::White;
			glm::vec4 BorderColor = VColor::Blue;
			float Radius = 10.0f;
			float Thickness = 2.0f;
			bool Hovering = false;
			bool Dragging = false;
		};
		struct Point
		{
			glm::vec2 Position { 0, 0 };
			ControlPoint ControlPoint {};

			bool Contains(const glm::vec2& point) const
			{
				float distance = glm::distance(Position, point);
				return distance <= ControlPoint.Radius;
			}
		};
		struct Data
		{
			std::list<Point> Points { Point{}, Point{} };
			glm::vec4 StrokeColor = VColor::Blue;
			glm::vec4 ArrowheadColor = VColor::Orange;
			float Thickness = 5.0f;
		};

		ArrowElement(CanvasCamera& camera)
			: m_Camera(camera) {}
		void Draw() override;
		void OnEvent(Event&) override;
		void MoveBy(float x, float y) override;
		bool Contains(const glm::vec2& point) const override;
		Box GetBoundingBox() const override;

		Data& GetData() { return m_Data; }
		float GetEndAngle() const;

	private:
		bool OnMousePressed(const Events::Input::MousePressed&);
		bool OnMouseReleased(const Events::Input::MouseReleased&);
		void HandleArrowEdit();

		const CanvasCamera& m_Camera;
		Data m_Data;

		static constexpr float EDIT_POINT_RADIUS = 10.0f;
	};
}