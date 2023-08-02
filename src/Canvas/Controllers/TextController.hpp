#pragma once

#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"

// TODO: Make new prefab for text
class TextController : public IController
{
public:
	TextController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;
	void OnEvent(Event& event) override;

private:
	void OnPasteEvent(const Events::Canvas::Paste&);
	void AddText(const std::string& text);

	EventQueue& m_EventQueue;
};

