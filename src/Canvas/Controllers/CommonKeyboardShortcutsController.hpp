#pragma once

#include "IController.hpp"
#include "Events/EventQueue.hpp"

class CommonKeyboardShortcutsController : public IController
{
public:
	CommonKeyboardShortcutsController(EventQueue& eventQueue) : m_EventQueue(eventQueue) {}
	void OnUpdate() override;

private:

	EventQueue& m_EventQueue;
};