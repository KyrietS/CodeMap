#include "TextElement.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "Input.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Utils/Strings.hpp"

namespace Elements
{
	void TextElement::Data::SetUtf8Text(const std::string& utf8Content)
	{
		Text = Utils::Strings::Utf8ToUnicode(utf8Content);
	}

	std::string TextElement::Data::GetTextInUtf8() const
	{
		return Utils::Strings::ToUtf8(Text);
	}

	void TextElement::Draw()
	{
		const bool showCursor = InEditMode and m_CursorVisible;
		Renderer::DrawText(m_Data.Text, m_Data.Position, m_Data.FontSize, m_Data.FontId, showCursor);
		if (InEditMode)
		{
			auto textMeasure = Renderer::MeasureText(m_Data.Text, m_Data.FontSize, m_Data.FontId);
			glm::vec2 topLeftCorner = {m_Data.Position.x + textMeasure.Offset.x, m_Data.Position.y + textMeasure.Offset.y};
			float thickness = 1.0f / m_Camera.GetZoom();
			Renderer::DrawRectangleLines(topLeftCorner, textMeasure.Size.x, textMeasure.Size.y, thickness, VColor::Orange);

			ToggleCursorVisibility();
		}
	}

	void TextElement::OnEvent(Event& event)
	{
		if (InEditMode)
		{
			EventDispatcher dispatcher(event);
			dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(OnMousePressed));
			dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(OnMouseReleased));
			dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(OnKeyPressed));
			dispatcher.Handle<Events::Input::KeyTyped>(BIND_EVENT(OnKeyTyped));
		}
		else
		{
			m_TextModeActive = false;
		}
	}

	bool TextElement::OnMousePressed(const Events::Input::MousePressed&)
	{
		const auto mousePos = Input::GetWorldMousePosition(m_Camera);
		const bool mouseOverText = GetBoundingBox().Contains(mousePos);
		if (mouseOverText and Input::IsMouseButtonDoublePressed(Mouse::ButtonLeft))
		{
			// Capture double click event by capturing both press and release events
			return true;
		}
		return false;
	}

	bool TextElement::OnMouseReleased(const Events::Input::MouseReleased&)
	{
		const auto mousePos = Input::GetWorldMousePosition(m_Camera);
		const bool mouseOverText = GetBoundingBox().Contains(mousePos);
		if (mouseOverText and Input::IsMouseButtonDoubleClicked(Mouse::ButtonLeft))
		{
			m_TextModeActive = true;
			return true;
		}
		else
		{
			m_TextModeActive = false;
		}

		return false;
	}

	bool TextElement::OnKeyPressed(const Events::Input::KeyPressed& key)
	{
		if (InEditMode)
		{
			auto& text = m_Data.Text;
			auto keyCode = key.GetKey();

			LOG_DEBUG("Key = {}", keyCode);
			if (keyCode == Key::Backspace and not text.empty())
			{
				text.pop_back();
				m_EventQueue.Push(Events::Canvas::MakeSnapshot {});
				return true;
			}
			if (keyCode == Key::Enter)
			{
				text.push_back('\n');
				m_EventQueue.Push(Events::Canvas::MakeSnapshot {});
				return true;
			}
		}
		return false;
	}

	bool TextElement::OnKeyTyped(const Events::Input::KeyTyped& key)
	{
		if (InEditMode)
		{
			LOG_DEBUG("Character = {}", key.GetCodePoint());
			m_Data.Text.push_back(key.GetCodePoint());
			m_EventQueue.Push(Events::Canvas::MakeSnapshot {});
			return true;
		}
		return false;
	}

	void TextElement::ToggleCursorVisibility()
	{
		constexpr float cursorBlinkTime = 0.5f;
		if (m_CursorTimer.Elapsed() >= cursorBlinkTime)
		{
			m_CursorVisible = not m_CursorVisible;
			m_CursorTimer.Reset();
		}
		// TODO: Instead of EmptyEvent, I should push a RedrawNeeded{} event
		// then handle it on App level and set some flag saying that when it's
		// set to true, push an EmptyEvent to the system and redraw everything.
		// The idea is that RedrawNeeded{} should be safe to push from OnEvent
		// function. Currently this would lead to infinite loop and no drawing.
		m_EventQueue.Push(EmptyEvent {});
	}

	void TextElement::MoveBy(float x, float y)
	{
		m_Data.Position += glm::vec2 { x, y };
	}

	Box TextElement::GetBoundingBox() const
	{
		const glm::vec2 padding = { 10.f, 10.f };
		const auto textMeasure = Renderer::MeasureText(m_Data.Text, m_Data.FontSize, m_Data.FontId);
		glm::vec2 corner = m_Data.Position + textMeasure.Offset - padding;
		glm::vec2 size = textMeasure.Size + 2.0f * padding;

		return {
			.x = corner.x,
			.y = corner.y,
			.width = size.x,
			.height = size.y
		};
	}
}