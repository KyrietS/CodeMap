#pragma once

#include "IController.hpp"
#include "Events/EventQueue.hpp"

// TODO: Make new prefab for text
class TextController : public IController
{
public:
	TextController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;

private:
	EventQueue& m_EventQueue;
};

