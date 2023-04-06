#pragma once
#include <glm/vec2.hpp>

struct ViewBox
{
	float X;
	float Y;
	float Width;
	float Height;
};

class ViewBoxBuilder
{
public:
	ViewBoxBuilder() = default;
	ViewBoxBuilder(const ViewBoxBuilder&) = delete;
	ViewBoxBuilder& operator=(const ViewBoxBuilder&) = delete;

	void AddPoint(glm::vec2 point)
	{
		m_MinX = std::min(m_MinX, point.x);
		m_MinY = std::min(m_MinY, point.y);
		m_MaxX = std::max(m_MaxX, point.x);
		m_MaxY = std::max(m_MaxY, point.y);
	}

	void AddPadding(int padding)
	{
		m_MinX -= padding;
		m_MinY -= padding;
		m_MaxX += padding;
		m_MaxY += padding;
	}

	ViewBox Build()
	{
		SetInvalidValuesToZero();
		return ViewBox{ m_MinX, m_MinY, m_MaxX - m_MinX, m_MaxY - m_MinY };
	}
private:
	void SetInvalidValuesToZero()
	{
		m_MinX = std::numeric_limits<float>::max() == m_MinX ? 0 : m_MinX;
		m_MinY = std::numeric_limits<float>::max() == m_MinY ? 0 : m_MinY;
		m_MaxX = std::numeric_limits<float>::min() == m_MaxX ? 0 : m_MaxX;
		m_MaxY = std::numeric_limits<float>::min() == m_MaxY ? 0 : m_MaxY;
	}

	float m_MinX = std::numeric_limits<float>::max();
	float m_MinY = std::numeric_limits<float>::max();
	float m_MaxX = std::numeric_limits<float>::min();
	float m_MaxY = std::numeric_limits<float>::min();
};