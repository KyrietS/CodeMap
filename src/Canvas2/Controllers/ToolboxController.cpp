#include "pch.hpp"
#include "ToolboxController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "Tools/HandController.hpp"
#include "Tools/SelectionController.hpp"
#include "Tools/ArrowController.hpp"
#include "Tools/ShapeController.hpp"
#include "Tools/TextController.hpp"

namespace Controllers
{
	void ToolboxController::Draw()
	{
		if (m_ActiveTool)
			m_ActiveTool->Draw();
	}
	void ToolboxController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::Canvas::SelectTool>(BIND_EVENT(ToolboxController::OnToolSelectedEvent));
		dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(ToolboxController::OnKeyPressed));

		if (m_ActiveTool and not event.Handled)
			m_ActiveTool->OnEvent(event);
	}

	void ToolboxController::OnToolSelectedEvent(const Events::Canvas::SelectTool& event)
	{
		m_EventQueue.Push(Events::Canvas::ClearFocus {});
		switch (event.Tool)
		{
		case ToolType::None:
			m_ActiveTool.reset();
			LOG_INFO("No tool selected");
			break;
		case ToolType::Hand:
			m_ActiveTool = std::make_unique<HandController>(m_Camera);
			LOG_INFO("Hand tool selected");
			break;
		case ToolType::Select:
			m_ActiveTool = std::make_unique<SelectionController>(m_Camera, m_Elements);
			LOG_INFO("Selection tool selected");
			break;
		case ToolType::Text:
			m_ActiveTool = std::make_unique<TextController>(m_Camera, m_Elements, m_EventQueue);
			LOG_INFO("Text tool selected");
			break;
		case ToolType::Arrow:
			m_ActiveTool = std::make_unique<ArrowController>(m_Camera, m_Elements);
			LOG_INFO("Arrow tool selected");
			break;
		case ToolType::Highlight:
			m_ActiveTool = std::make_unique<ShapeController>(m_Camera, m_Elements);
			LOG_INFO("Shape tool selected");
			break;
		default:
			LOG_WARN("Unknown tool type: {}", static_cast<int>(event.Tool));
		}
	}

	bool ToolboxController::OnKeyPressed(const Events::Input::KeyPressed& event)
	{
		switch (event.GetKey())
		{
		case Key::D1:
			m_EventQueue.Push(Events::Canvas::SelectTool { ToolType::Hand });
			return true;
		case Key::D2:
			m_EventQueue.Push(Events::Canvas::SelectTool { ToolType::Select });
			return true;
		case Key::D3:
			m_EventQueue.Push(Events::Canvas::SelectTool { ToolType::Arrow });
			return true;
		case Key::D4:
			m_EventQueue.Push(Events::Canvas::SelectTool { ToolType::Highlight });
			return true;
		case Key::D5:
			m_EventQueue.Push(Events::Canvas::SelectTool { ToolType::Text });
			return true;
		}

		return false;
	}

}