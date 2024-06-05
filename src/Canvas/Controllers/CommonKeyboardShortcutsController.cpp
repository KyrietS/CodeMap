#include "pch.hpp"
#include "Canvas/Controllers/CommonKeyboardShortcutsController.hpp"
#include "Input.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"

namespace Controllers
{
	void CommonKeyboardShortcutsController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(OnKeyPressed));
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

		return false;
	}
}