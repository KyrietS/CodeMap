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
		struct Data
		{
			glm::vec2 Begin { 0, 0 };
			glm::vec2 End { 0, 0 };
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
		float GetLength() const;
		float GetEndAngle() const;

	private:
		const CanvasCamera& m_Camera;
		Data m_Data;
	};
}