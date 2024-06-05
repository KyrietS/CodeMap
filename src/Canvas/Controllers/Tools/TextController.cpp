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
	}

	bool TextController::OnMouseReleased(Events::Input::MouseReleased& event)
	{
		if (Input::IsMouseButtonClicked(Mouse::ButtonLeft))
		{
			AddTextToCanvas();
			return true;
		}

		return false;
	}

	void TextController::AddTextToCanvas()
	{
		auto text = std::make_unique<Elements::TextElement>(m_Camera, m_EventQueue);
		text->GetData().Position = Input::GetWorldMousePosition(m_Camera);
		text->GetData().SetUtf8Text("Hello, World!");

		m_Elements.Add(std::move(text));
		m_EventQueue.Push(Events::Canvas::MakeSnapshot {});
	}

}