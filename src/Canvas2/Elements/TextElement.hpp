#pragma once
#include "IElement.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "Events/MouseEvents.hpp"
#include "Render/BlendMode.hpp"
#include "ControlPoint.hpp"

class CanvasCamera;

namespace Elements
{
	class TextElement : public IElement
	{
	public:
		struct Data
		{
			glm::vec2 Position = { 0.0f, 0.0f };
			std::vector<uint32_t> Text = {}; // In UTF-32 (Unicode codepoints)
			float FontSize = 32.0f;
			unsigned int FontId = 0;

			void SetUtf8Text(const std::string& utf8Content);
			std::string GetTextInUtf8() const;
		};

		TextElement(CanvasCamera& camera)
			: m_Camera(camera) {}
		void Draw() override;
		void OnEvent(Event&) override;
		void MoveBy(float x, float y) override;
		Box GetBoundingBox() const override;

		Data& GetData() { return m_Data; }
	private:
		const CanvasCamera& m_Camera;
		Data m_Data;
	};
}