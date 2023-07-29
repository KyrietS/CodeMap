#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"

// TODO: Change to ClipboardController
class ImageController : public IController
{
public:
	ImageController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;

private:
	void PasteImageFromClipboard();

	EventQueue& m_EventQueue;
};

