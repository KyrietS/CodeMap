#pragma once

#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Events/KeyEvents.hpp"

namespace Controllers
{

	class CommonKeyboardShortcutsController : public IController
	{
	public:
		CommonKeyboardShortcutsController(EventQueue& eventQueue) : m_EventQueue(eventQueue) {}
		void OnEvent(Event&) override;

	private:
		bool OnKeyPressed(const Events::Input::KeyPressed&);

		EventQueue& m_EventQueue;
	};
}