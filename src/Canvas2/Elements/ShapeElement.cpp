#include "ArrowElement.hpp"
#include <glm/gtx/vector_angle.hpp>
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "ShapeElement.hpp"
#include <ranges>

namespace Elements
{
	namespace
	{
		template <std::ranges::input_range Range>
		void printTransformedVec2(Range&& range) {
			LOG_INFO("Size: {}", range.size());
			for (const auto& vec : range) {
				LOG_INFO("x: {0}, y: {1}", vec.x, vec.y);
			}
		}
	}

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
					return;
				point.OnEvent(event);
			}
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
		m_Data.Points.emplace_back(point, m_Camera);
	}
}