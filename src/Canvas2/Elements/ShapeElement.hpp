#pragma once
#include "IElement.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/EventQueue.hpp"
#include "Render/BlendMode.hpp"
#include "ControlPoint.hpp"

class CanvasCamera;

namespace Elements
{
	class ShapeElement : public IElement
	{
	public:
		struct Data
		{
			std::vector<ControlPoint> Points = {};
			glm::vec4 Color = VColor::Yellow;
			Render::BlendMode BlendMode = Render::BlendMode::Multiply;
		};

		ShapeElement(CanvasCamera& camera, EventQueue& eventQueue)
			: m_Camera(camera), m_EventQueue(eventQueue) {}
		void Draw() override;
		void OnEvent(Event&) override;
		void MoveBy(float x, float y) override;
		bool Contains(const glm::vec2& point) const override;
		Box GetBoundingBox() const override;

		Data& GetData() { return m_Data; }
		const Data& GetData() const { return m_Data; }
		void AddPoint(const glm::vec2& point);

	private:
		void OnUpdate();

		const CanvasCamera& m_Camera;
		EventQueue& m_EventQueue;
		Data m_Data;

		bool m_IsRectangle = true;
	};
}