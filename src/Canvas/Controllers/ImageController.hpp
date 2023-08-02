#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"

// TODO: Change to ClipboardController
class ImageController : public IController
{
public:
	ImageController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnEvent(Event& event) override;

private:
	void OnPasteEvent(const Events::Canvas::Paste&);
	void PasteImageFromClipboard();

	EventQueue& m_EventQueue;
};

