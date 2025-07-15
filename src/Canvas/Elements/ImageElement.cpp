#include "ArrowElement.hpp"
#include <glm/vec2.hpp>
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "ShapeElement.hpp"
#include "ImageElement.hpp"

namespace Elements
{
	void ImageElement::Draw()
	{
		Renderer::DrawImage(m_Data.Position, m_Data.Width, m_Data.Height, *m_Data.TextureId);
	}

	void ImageElement::MoveBy(float x, float y)
	{
		m_Data.Position += glm::vec2(x, y);
	}

	Box ImageElement::GetBoundingBox() const
	{
		return {
			.x = m_Data.Position.x,
			.y = m_Data.Position.y,
			.width = (float)m_Data.Width,
			.height = (float)m_Data.Height
		};
	}


}