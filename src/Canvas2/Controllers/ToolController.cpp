#include "pch.hpp"
#include "ToolController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "HandController.hpp"

namespace Controllers
{
	void ToolController::Draw()
	{
		if (m_ActiveTool)
			m_ActiveTool->Draw();
	}
	void ToolController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Events::Canvas::SelectTool>(BIND_EVENT(ToolController::OnToolSelectedEvent));

		if (m_ActiveTool)
			m_ActiveTool->OnEvent(event);
	}

	void ToolController::OnToolSelectedEvent(const Events::Canvas::SelectTool& event)
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
			m_ActiveTool.reset();
			LOG_WARN("Select tool not implemented yet!");
			break;
		case ToolType::Text:
			m_ActiveTool.reset();
			LOG_WARN("Text tool not implemented yet!");
			break;
		case ToolType::Arrow:
			m_ActiveTool.reset();
			LOG_WARN("Arrow tool not implemented yet!");
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