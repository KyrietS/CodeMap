#pragma once

#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Events/KeyEvents.hpp"
#include "Events/CanvasEvents.hpp"

namespace Controllers
{

	class CommonKeyboardShortcutsController : public IController
	{
	public:
		CommonKeyboardShortcutsController(EventQueue& eventQueue) : m_EventQueue(eventQueue) {}
		void OnEvent(Event&) override;

	private:
		bool OnKeyPressed(const Events::Input::KeyPressed&);
		bool OnPaste(const Events::Canvas::Paste&);

		EventQueue& m_EventQueue;
	};
}