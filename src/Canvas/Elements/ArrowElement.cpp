#include "ArrowElement.hpp"
#define GLM_ENABLE_EXPERIMENTAL
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
		glm::vec2 begin = m_Data.Points.front().Position;
		glm::vec2 end = m_Data.Points.back().Position;

		for (auto it = m_Data.Points.begin(); it != std::prev(m_Data.Points.end()); ++it)
		{
			auto next = std::next(it);
			Renderer::DrawLine(it->Position, next->Position, m_Data.Thickness, m_Data.StrokeColor);
		}



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
		if (InEditMode)
		{
			for (auto& point : m_Data.Points)
			{
				if (event.Handled)
					return;
				point.OnEvent(event);
			}
		}
	}

	void ArrowElement::MoveBy(float x, float y)
	{
		for (auto& point : m_Data.Points)
		{
			point.Position += glm::vec2 { x, y };
		}
	}

	bool ArrowElement::Contains(const glm::vec2& point) const
	{
		return GetBoundingBox().Contains(point);
	}

	Box ArrowElement::GetBoundingBox() const
	{
		float minX = m_Data.Points.front().Position.x;
		float minY = m_Data.Points.front().Position.y;
		float maxX = m_Data.Points.front().Position.x;
		float maxY = m_Data.Points.front().Position.y;
		for (const auto& point : m_Data.Points)
		{
			minX = std::min(minX, point.Position.x);
			minY = std::min(minY, point.Position.y);
			maxX = std::max(maxX, point.Position.x);
			maxY = std::max(maxY, point.Position.y);
		}
		float width = std::max(std::abs(maxX - minX), 5.0f);
		float height = std::max(std::abs(maxY - minY), 5.0f);

		// TODO: Instead of max(width, 5.0f) use arrow thickness
		return Box {
			.x = minX,
			.y = minY,
			.width = width,
			.height = height
		};
	}

	float ArrowElement::GetEndAngle() const
	{
		assert(m_Data.Points.size() >= 2);
		glm::vec2 end = m_Data.Points.back().Position;

		// find previous point with different position
		glm::vec2 prevEnd = end;
		for (auto& point : m_Data.Points | std::views::reverse)
		{
			if (point.Position != end)
			{
				prevEnd = point.Position;
				break;
			}
		}

		return glm::orientedAngle(glm::vec2 { 1.0f, 0.0f }, glm::normalize(end - prevEnd));
	}
}