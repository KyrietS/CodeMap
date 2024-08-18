#include "pch.hpp"
#include "TextController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas/Elements/TextElement.hpp"
#include "Input.hpp"

namespace Controllers
{
	TextController::TextController(CanvasCamera& camera, CanvasElements& elements, EventQueue& eventQueue)
		: m_Camera(camera), m_Elements(elements), m_EventQueue(eventQueue)
	{
	}

	void TextController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(OnMouseReleased));
		dispatcher.Handle<Events::Canvas::PasteText>(BIND_EVENT(OnPasteText));
	}

	bool TextController::OnMouseReleased(Events::Input::MouseReleased& event)
	{
		if (Input::IsMouseButtonClicked(Mouse::ButtonLeft))
		{
			AddTextToCanvas("");
			return true;
		}

		return false;
	}

	bool TextController::OnPasteText(Events::Canvas::PasteText& event)
	{
		LOG_DEBUG("Pasted text: {}", event.Text);
		AddTextToCanvas(event.Text);
		return true;
	}

	void TextController::AddTextToCanvas(const std::string& text)
	{
		auto textElement = std::make_unique<Elements::TextElement>(m_Camera, m_EventQueue);
		textElement->GetData().Position = Input::GetWorldMousePosition(m_Camera);
		textElement->GetData().SetUtf8Text(text);

		ElementId id = m_Elements.Add(std::move(textElement));
		m_EventQueue.Push(Events::Canvas::SelectElement { id });
		m_EventQueue.Push(Events::Canvas::SelectTool { ToolType::Select });
		m_EventQueue.Push(Events::Canvas::MakeSnapshot {});
	}

}