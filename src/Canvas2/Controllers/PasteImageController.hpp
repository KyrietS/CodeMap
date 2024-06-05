#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas2/CanvasElements.hpp"

namespace Controllers
{
class PasteImageController : public IController
{
public:
	PasteImageController(CanvasCamera& camera, EventQueue& eventQueue, CanvasElements& elements)
		: m_Camera(camera), m_EventQueue(eventQueue), m_Elements(elements) {}
	void OnEvent(Event& event) override;

private:
	bool OnPaste(const Events::Canvas::Paste&);
	bool HasImageInClipboard();
	void AddImageFromClipboardToCanvas();

	CanvasCamera& m_Camera;
	EventQueue& m_EventQueue;
	CanvasElements& m_Elements;
};

}
