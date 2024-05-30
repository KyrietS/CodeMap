#include "pch.hpp"
#include "ArrowController.hpp"
#include "Canvas2/Elements/ArrowElement.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Window.hpp"

namespace Controllers
{
	ArrowController::ArrowController(CanvasCamera& camera, CanvasElements& elements)
		: m_Camera(camera), m_Elements(elements)
	{
	}

	void ArrowController::Draw()
	{
		if (m_Arrow)
		{
			m_Arrow->Draw();
		}
	}

	void ArrowController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(OnMousePressed));
		dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(OnMouseReleased));

		OnUpdate();
	}

	void ArrowController::OnUpdate()
	{
		Window::GetMouseCursor().SetArrowImage();

		if (m_Arrow)
		{
			m_Arrow->GetData().Points.back().Position = Input::GetWorldMousePosition(m_Camera);
		}
	}

	bool ArrowController::OnMousePressed(Events::Input::MousePressed& event)
	{
		if (event.GetButton() != Mouse::ButtonLeft)
			return false;

		if (not m_Arrow)
		{
			m_Arrow = std::make_unique<Elements::ArrowElement>(m_Camera);

			glm::vec2 begin = Input::GetWorldMousePosition(m_Camera);
			m_Arrow->GetData().Points.emplace_back(begin, m_Camera);
			m_Arrow->GetData().Points.emplace_back(begin, m_Camera);
		}
		else
		{
			AddArrowToCanvas();
		}

		return true;
	}

	bool ArrowController::OnMouseReleased(Events::Input::MouseReleased& event)
	{
		if (event.GetButton() != Mouse::ButtonLeft)
			return false;

		if (m_Arrow and not Input::IsMouseButtonClicked(Mouse::ButtonLeft))
		{
			AddArrowToCanvas();
			return true;
		}

		return true;
	}

	void ArrowController::AddArrowToCanvas()
	{
		auto box = m_Arrow->GetBoundingBox();
		bool isTooSmall = box.width < 5.0f and box.height < 5.0f;
		if (m_Arrow and not isTooSmall)
		{
			LOG_DEBUG("Arrow added to canvas");
			m_Elements.Add(std::move(m_Arrow));
		}
		m_Arrow.reset();
	}
}