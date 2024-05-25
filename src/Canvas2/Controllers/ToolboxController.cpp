#include "pch.hpp"
#include "ToolboxController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "Tools/HandController.hpp"
#include "Tools/SelectionController.hpp"
#include "Tools/ArrowController.hpp"

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

		if (m_ActiveTool)
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
			m_ActiveTool.reset();
			LOG_WARN("Text tool not implemented yet!");
			break;
		case ToolType::Arrow:
			m_ActiveTool = std::make_unique<ArrowController>(m_Camera, m_Elements);
			LOG_INFO("Arrow tool selected");
			break;
		case ToolType::Highlight:
			m_ActiveTool.reset();
			LOG_WARN("Highlight tool not implemented yet!");
			break;
		default:
			LOG_WARN("Unknown tool type: {}", static_cast<int>(event.Tool));
		}
	}

}