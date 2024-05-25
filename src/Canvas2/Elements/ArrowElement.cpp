#include "ArrowElement.hpp"
#include <glm/gtx/vector_angle.hpp>
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Canvas/CanvasCamera.hpp"

namespace Elements
{
	void ArrowElement::Draw()
	{
		glm::vec2 begin = m_Data.Begin;
		glm::vec2 end = m_Data.End;

		Renderer::DrawLine(begin, end, m_Data.Thickness, m_Data.StrokeColor);

		// Arrowhead directed to the right and pointing at (0,0)
		float arrowheadSize = 30.0f * m_Data.Thickness / 5;
		glm::vec2 tip1 { -arrowheadSize, -arrowheadSize / 3 };
		glm::vec2 tip2 { -arrowheadSize, +arrowheadSize / 3 };
		glm::vec2 tip3 { 0.0f, 0.0f };

		// Rotate relative to curvature
		float angle = GetEndAngle();
		tip1 = glm::rotate(tip1, angle);
		tip2 = glm::rotate(tip2, angle);
		tip3 = glm::rotate(tip3, angle); // redundant rotation of (0,0)

		// Translate
		tip1 += end;
		tip2 += end;
		tip3 += end;

		Renderer::DrawTriangle(tip1, tip2, tip3, m_Data.ArrowheadColor);
	}
	void ArrowElement::OnEvent(Event& event)
	{
	}

	void ArrowElement::MoveBy(float x, float y)
	{
		m_Data.Begin += glm::vec2 { x, y };
		m_Data.End += glm::vec2 { x, y };
	}

	bool ArrowElement::Contains(const glm::vec2& point) const
	{
		return GetBoundingBox().Contains(point);
	}

	Box ArrowElement::GetBoundingBox() const
	{
		// TODO: Instead of max(width, 5.0f) use arrow thickness
		return Box {
			.x = std::min(m_Data.Begin.x, m_Data.End.x),
			.y = std::min(m_Data.Begin.y, m_Data.End.y),
			.width = std::max(std::abs(m_Data.End.x - m_Data.Begin.x), 5.0f),
			.height = std::max(std::abs(m_Data.End.y - m_Data.Begin.y), 5.0f)
		};
	}

	float ArrowElement::GetLength() const
	{
		return glm::length(m_Data.End - m_Data.Begin);
	}
	float ArrowElement::GetEndAngle() const
	{
		glm::vec2 control = { 0.0f, 0.0f }; // TODO: take curvature into account
		return glm::orientedAngle(glm::vec2 { 1.0f, 0.0f }, glm::normalize(m_Data.End - m_Data.Begin/*control*/));
	}
}