#include "ArrowElement.hpp"
#include <glm/gtx/vector_angle.hpp>
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "ShapeElement.hpp"

namespace Elements
{
	void Elements::ShapeElement::Draw()
	{
		Renderer::BeginBlendMode(m_Data.BlendMode);
		Renderer::DrawPolygon(m_Data.Points, m_Data.Color);
		Renderer::EndBlendMode();
	}

	void ShapeElement::OnEvent(Event&)
	{
	}

	void ShapeElement::MoveBy(float x, float y)
	{
		for (auto& point : m_Data.Points)
		{
			point += glm::vec2 { x, y };
		}
	}

	bool ShapeElement::Contains(const glm::vec2& point) const
	{
		return GetBoundingBox().Contains(point);
	}

	// TODO: Create a library class for finding
	// the bounding box of a polygon
	Box ShapeElement::GetBoundingBox() const
	{
		float minX = m_Data.Points[0].x;
		float minY = m_Data.Points[0].y;
		float maxX = m_Data.Points[0].x;
		float maxY = m_Data.Points[0].y;

		for (const auto& point : m_Data.Points)
		{
			minX = std::min(minX, point.x);
			minY = std::min(minY, point.y);
			maxX = std::max(maxX, point.x);
			maxY = std::max(maxY, point.y);
		}

		return Box {
			.x = minX,
			.y = minY,
			.width = maxX - minX,
			.height = maxY - minY
		};
	}
}