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
			float LetterSpacing = 0.0f;
			glm::vec4 FontColor = VColor::Black;
			unsigned int FontId = 0;

			void SetUtf8Text(const std::string& utf8Content);
			std::string GetTextInUtf8() const;
		};

		TextElement(CanvasCamera& camera, EventQueue& eventQueue)
			: m_Camera(camera), m_EventQueue(eventQueue), m_Data(GetDefaultData()) {}
		void Draw() override;
		void OnEvent(Event&) override;
		void MoveBy(float x, float y) override;
		Box GetBoundingBox() const override;
		void Accept(IElementVisitor& visitor) override;

		Data& GetData() { return m_Data; }
		const Data& GetData() const { return m_Data; }

		static Data& GetDefaultData() {
			static Data defaultData;
			return defaultData;
		}

	private:
		bool OnMousePressed(const Events::Input::MousePressed&);
		bool OnMouseReleased(const Events::Input::MouseReleased&);
		bool OnKeyPressed(const Events::Input::KeyPressed&);
		bool OnKeyTyped(const Events::Input::KeyTyped&);
		void ToggleCursorVisibility();

		const CanvasCamera& m_Camera;
		EventQueue& m_EventQueue;
		Data m_Data;
		bool m_TextModeActive = false;
		bool m_CursorVisible = false;
		Timer m_CursorTimer;
	};
}