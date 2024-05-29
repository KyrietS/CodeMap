#pragma once
#include "IElement.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"
#include "Render/BlendMode.hpp"
#include "ControlPoint.hpp"
#include "Timer.hpp"
#include "Events/EventQueue.hpp"

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

		TextElement(CanvasCamera& camera, EventQueue& eventQueue)
			: m_Camera(camera), m_EventQueue(eventQueue) {}
		void Draw() override;
		void OnEvent(Event&) override;
		void MoveBy(float x, float y) override;
		Box GetBoundingBox() const override;

		Data& GetData() { return m_Data; }
	private:
		bool OnMousePressed(const Events::Input::MousePressed&);
		bool OnMouseReleased(const Events::Input::MouseReleased&);
		bool OnKeyPressed(const Events::Input::KeyPressed&);
		bool OnKeyTyped(const Events::Input::KeyTyped&);
		void ToggleCursorVisibility();

		const CanvasCamera& m_Camera;
		Data m_Data;
		bool m_TextModeActive = false;
		bool m_CursorVisible = false;
		EventQueue& m_EventQueue;
		Timer m_CursorTimer;
	};
}