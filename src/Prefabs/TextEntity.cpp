#include "pch.hpp"
#include "TextEntity.hpp"
#include "Canvas/Components.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Utils/Strings.hpp"
#include "Utils/System.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/CanvasEvents.hpp"
#include "Timer.hpp"

namespace
{
	void UpdateTextFocusArea(Components::Focusable& focus, const Components::Text& text)
	{
		const glm::vec2 padding = { 10.0f, 10.0f };
		const auto textMeasurement = Renderer::MeasureText(text);
		focus.Size = textMeasurement.Size + 2.0f * padding;
		focus.Origin = textMeasurement.Offset - padding;
	}

	struct Script : ScriptableEntity
	{
		Script(EventQueue& eventQueue)
			: m_EventQueue(eventQueue)
		{
		}

		void OnUpdate() override
		{
			auto& focus = GetComponent<Components::Focusable>();
			auto& text = GetComponent<Components::Text>();

			if (focus.IsFocused && Input::IsMouseButtonDoubleClicked(Mouse::ButtonLeft))
			{
				Input::BeginTextMode();
				m_IsTextModeActive = true;
				m_CursorTimer.Reset();
				focus.BorderColor = VColor::Orange;
			}

			if (m_IsTextModeActive)
			{
				toggleCursorVisibility();
				auto& content = text.Content;
				auto contentBeforeChange = content;
				while (uint32_t character = Input::GetChar())
				{
					LOG_DEBUG("Character = {}", character);
					content.push_back(character);
				}
				while (KeyCode keyCode = Input::GetKey())
				{
					LOG_DEBUG("Key code = {}", keyCode);
					if (keyCode == Key::Backspace && !content.empty())
						content.pop_back();
					if (keyCode == Key::Enter)
						content.push_back('\n');
				}

				if (contentBeforeChange != content)
				{
					m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
				}
			}

			if (m_IsTextModeActive && !focus.IsFocused)
			{
				Input::EndTextMode();
				m_IsTextModeActive = false;
				focus.ResetBorderColor();
				text.IsCursorShown = false;
			}

			UpdateTextFocusArea(focus, text);
		}

		void toggleCursorVisibility()
		{
			auto& isCursorShown = GetComponent<Components::Text>().IsCursorShown;
			constexpr float cursorToggleInterval = 0.5f;
			if (m_CursorTimer.Elapsed() >= cursorToggleInterval)
			{
				isCursorShown = !isCursorShown;
				m_CursorTimer.Reset();
			}
			m_EventQueue.Push(EmptyEvent{});
		}

		bool m_IsTextModeActive = false;
		EventQueue& m_EventQueue;
		Timer m_CursorTimer;
	};
}


TextEntity::TextEntity(const Entity& entity, EventQueue& eventQueue)
	: Entity(entity)
{
	AddComponent<Components::Text>();
	AddComponent<Components::Focusable>();

	AttachScript<::Script>(std::ref(eventQueue));
	AttachScript<MoveByDragScript>(std::ref(eventQueue));
}

TextEntity& TextEntity::Build(const std::string& content, float fontSize)
{
	auto& text = GetComponent<Components::Text>();
	text = Components::Text{ {}, fontSize, 0.0f, VColor::Black };
	text.SetUtf8Content(content);

	auto& focus = GetComponent<Components::Focusable>();
	UpdateTextFocusArea(focus, text);

	return *this;
}
