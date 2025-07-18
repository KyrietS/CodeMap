#pragma once
#include "IElement.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "ControlPoint.hpp"

class CanvasCamera;

namespace Elements
{
	class ArrowElement : public IElement
	{
	public:
		struct Data
		{
			std::list<ControlPoint> Points {};
			glm::vec4 StrokeColor = VColor::Blue;
			glm::vec4 ArrowheadColor = VColor::Orange;
			float Thickness = 5.0f;
		};

		ArrowElement(CanvasCamera& camera)
			: m_Camera(camera), m_Data(GetDefaultData()) {}
		void Draw() override;
		void OnEvent(Event&) override;
		void MoveBy(float x, float y) override;
		bool Contains(const glm::vec2& point) const override;
		Box GetBoundingBox() const override;
		void Accept(IElementVisitor& visitor) override;

		Data& GetData() { return m_Data; }
		const Data& GetData() const { return m_Data; }
		float GetEndAngle() const;

		static Data& GetDefaultData() {
			static Data defaultData;
			return defaultData;
		}

	private:
		const CanvasCamera& m_Camera;
		Data m_Data;

	};
}