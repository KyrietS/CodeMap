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
		dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(OnKeyPressed));

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
			return true;
		}

		if (m_Arrow)
		{
			if (Input::IsMouseButtonDoublePressed(Mouse::ButtonLeft))
			{
				m_Arrow->GetData().Points.pop_back();
				AddArrowToCanvas();
			}
			else
			{
				auto mousePos = Input::GetWorldMousePosition(m_Camera);
				m_Arrow->GetData().Points.emplace_back(mousePos, m_Camera);
			}
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

	bool ArrowController::OnKeyPressed(Events::Input::KeyPressed& event)
	{
		if (event.GetKey() == Key::Escape)
		{
			m_Arrow.reset();
			return true;
		}
		if (event.GetKey() == Key::Space)
		{
			m_Arrow->GetData().Points.pop_back();
			AddArrowToCanvas();
		}
		return false;
	}

	void ArrowController::AddArrowToCanvas()
	{
		auto box = m_Arrow->GetBoundingBox();
		bool isTooSmall = box.width < 30.0f and box.height < 30.0f;
		bool enoughPoints = m_Arrow->GetData().Points.size() >= 2;
		if (isTooSmall)
		{
			LOG_INFO("Arrow is too small to be added to canvas");
		}
		else if (not enoughPoints)
		{
			LOG_INFO("Arrow must have at least 2 points to be added to canvas");
		}
		else if (m_Arrow)
		{
			LOG_DEBUG("Arrow added to canvas");
			m_Elements.Add(std::move(m_Arrow));
		}

		m_Arrow.reset();
	}
}