#include "pch.hpp"
#include "Canvas/Controllers/CommonKeyboardShortcutsController.hpp"
#include "Input.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"
#include <clip.h>

namespace Controllers
{
	void CommonKeyboardShortcutsController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(OnKeyPressed));
		dispatcher.Handle<Events::Canvas::Paste>(BIND_EVENT(OnPaste));
	}

	bool CommonKeyboardShortcutsController::OnKeyPressed(const Events::Input::KeyPressed& event)
	{
		// Ctrl + Z
		if (Input::IsKeyDown(Key::LeftControl) && event.GetKey() == Key::Z)
		{
			LOG_DEBUG("Ctrl + Z");
			m_EventQueue.Push(Events::Canvas::Undo {});
			return true;
		}

		// Ctrl + Y
		if (Input::IsKeyDown(Key::LeftControl) && event.GetKey() == Key::Y)
		{
			LOG_DEBUG("Ctrl + Y");
			m_EventQueue.Push(Events::Canvas::Redo {});
			return true;
		}

		// Ctrl + X
		if (Input::IsKeyDown(Key::LeftControl) && event.GetKey() == Key::X)
		{
			LOG_DEBUG("Ctrl + X");
			m_EventQueue.Push(Events::Canvas::Cut {});
			return true;
		}

		// Ctrl + C
		if (Input::IsKeyDown(Key::LeftControl) && event.GetKey() == Key::C)
		{
			LOG_DEBUG("Ctrl + C");
			m_EventQueue.Push(Events::Canvas::Copy {});
			return true;
		}

		// Ctrl + V
		if (Input::IsKeyDown(Key::LeftControl) && event.GetKey() == Key::V)
		{
			LOG_DEBUG("Ctrl + V");
			m_EventQueue.Push(Events::Canvas::Paste {});
			return true;
		}

		// Ctrl + S
		if (Input::IsKeyDown(Key::LeftControl) && event.GetKey() == Key::S)
		{
			LOG_DEBUG("Ctrl + S");
			m_EventQueue.Push(Events::Canvas::SaveToFile {});
			return true;
		}

		return false;
	}

	bool CommonKeyboardShortcutsController::OnPaste(const Events::Canvas::Paste&)
	{
		if (clip::has(clip::text_format()))
		{
			std::string pastedText;
			clip::get_text(pastedText);
			m_EventQueue.Push(Events::Canvas::SelectTool{ToolType::Text});
			m_EventQueue.Push(Events::Canvas::PasteText{std::move(pastedText)});
			return true;
		}

		return false;
	}
}