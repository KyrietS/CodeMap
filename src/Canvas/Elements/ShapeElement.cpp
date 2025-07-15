#include "ArrowElement.hpp"
#include <glm/vec2.hpp>
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "ShapeElement.hpp"
#include <ranges>

namespace Elements
{
	void Elements::ShapeElement::Draw()
	{
		Renderer::BeginBlendMode(m_Data.BlendMode);
		{
			// Sweet jeez, allocating a vector for each draw call is a terrible idea
			auto positions = m_Data.Points | std::views::transform([](const ControlPoint& point) { return point.Position; });
			std::vector<glm::vec2> points = { positions.begin(), positions.end() };
			Renderer::DrawPolygon(points, m_Data.Color);
		}
		Renderer::EndBlendMode();

		if (InEditMode)
		{
			for (const auto& point : m_Data.Points)
			{
				point.Draw(m_Camera);
			}
		}
	}

	void ShapeElement::OnEvent(Event& event)
	{
		if (InEditMode)
		{
			for (auto& point : m_Data.Points)
			{
				if (event.Handled)
					break;
				point.OnEvent(event);
			}

			OnUpdate();
		}
	}

	void ShapeElement::MoveBy(float x, float y)
	{
		for (auto& point : m_Data.Points)
		{
			point.Position += glm::vec2 { x, y };
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
		assert(m_Data.Points.size() > 0);
		float minX = m_Data.Points.front().Position.x;
		float minY = m_Data.Points.front().Position.y;
		float maxX = m_Data.Points.front().Position.x;
		float maxY = m_Data.Points.front().Position.y;

		for (const auto& point : m_Data.Points)
		{
			auto pos = point.Position;
			minX = std::min(minX, pos.x);
			minY = std::min(minY, pos.y);
			maxX = std::max(maxX, pos.x);
			maxY = std::max(maxY, pos.y);
		}

		return Box {
			.x = minX,
			.y = minY,
			.width = maxX - minX,
			.height = maxY - minY
		};
	}
	void ShapeElement::AddPoint(const glm::vec2& point)
	{
		m_Data.Points.emplace_back(point, m_Camera, m_EventQueue);
	}

	void ShapeElement::OnUpdate()
	{
		if (m_IsRectangle)
		{
			// Keep the rectangle shape by aligning other corners to the dragged corner
			assert(m_Data.Points.size() == 4);
			auto& point1 = m_Data.Points[0]; // Top left
			auto& point2 = m_Data.Points[1]; // Top right
			auto& point3 = m_Data.Points[2]; // Bottom right
			auto& point4 = m_Data.Points[3]; // Bottom left

			if (point1.Dragging)
			{
				point2.Position.y = point1.Position.y;
				point4.Position.x = point1.Position.x;
			}
			else if (point2.Dragging)
			{
				point1.Position.y = point2.Position.y;
				point3.Position.x = point2.Position.x;
			}
			else if (point3.Dragging)
			{
				point2.Position.x = point3.Position.x;
				point4.Position.y = point3.Position.y;
			}
			else if (point4.Dragging)
			{
				point1.Position.x = point4.Position.x;
				point3.Position.y = point4.Position.y;
			}
		}
	}
}