#pragma once
#include "IElement.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "Events/MouseEvents.hpp"
#include "Render/BlendMode.hpp"

class CanvasCamera;

namespace Elements
{
	class ShapeElement : public IElement
	{
	public:
		struct Data
		{
			glm::vec4 Color = VColor::Yellow;
			Render::BlendMode BlendMode = Render::BlendMode::Multiply;
			std::vector<glm::vec2> Points = {};
		};

		ShapeElement(CanvasCamera& camera)
			: m_Camera(camera) {}
		void Draw() override;
		void OnEvent(Event&) override;
		void MoveBy(float x, float y) override;
		bool Contains(const glm::vec2& point) const override;
		Box GetBoundingBox() const override;

		Data& GetData() { return m_Data; }

	private:
		const CanvasCamera& m_Camera;
		Data m_Data;
	};
}