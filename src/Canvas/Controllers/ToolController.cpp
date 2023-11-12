#include "pch.hpp"
#include "ToolController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Canvas/Controllers/LineController.hpp"
#include "Canvas/Controllers/HighlightController.hpp"
#include "Canvas/Controllers/TextController.hpp"
#include "Canvas/Controllers/SelectionController.hpp"
#include "Canvas/Controllers/HandToolController.hpp"

void ToolController::OnUpdate()
{
	for (auto& s_ActiveTool : m_ActiveTools)
		s_ActiveTool->OnUpdate();
}

void ToolController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Canvas::SelectTool>(BIND_EVENT(ToolController::OnToolSelectedEvent));
	for (auto& m_ActiveTool : m_ActiveTools)
		m_ActiveTool->OnEvent(event);
}

void ToolController::OnToolSelectedEvent(const Events::Canvas::SelectTool &event)
{
	m_EventQueue.Push(Events::Canvas::ClearFocus{});
	switch(event.Tool)
	{
		case ToolType::None:
			m_ActiveTools.clear();
			LOG_INFO("No tool selected");
			break;
		case ToolType::Hand:
			m_ActiveTools.clear();
			m_ActiveTools.push_back(std::make_unique<HandToolController>(m_Camera));
			LOG_INFO("Hand tool selected");
			break;
		case ToolType::Select:
			m_ActiveTools.clear();
			m_ActiveTools.push_back(std::make_unique<SelectionController>(m_EventQueue));
			LOG_INFO("Select tool selected");
			break;
		case ToolType::Text:
			m_ActiveTools.clear();
			m_ActiveTools.push_back(std::make_unique<SelectionController>(m_EventQueue));
			m_ActiveTools.push_back(std::make_unique<TextController>(m_EventQueue));
			LOG_INFO("Text tool selected");
			break;
		case ToolType::Arrow:
			m_ActiveTools.clear();
			m_ActiveTools.push_back(std::make_unique<SelectionController>(m_EventQueue));
			m_ActiveTools.push_back(std::make_unique<LineController>(m_EventQueue));
			LOG_INFO("Arrow tool selected");
			break;
		case ToolType::Highlight:
			m_ActiveTools.clear();
			m_ActiveTools.push_back(std::make_unique<SelectionController>(m_EventQueue));
			m_ActiveTools.push_back(std::make_unique<HighlightController>(m_EventQueue));
			LOG_INFO("Highlight tool selected");
			break;
		default:
			LOG_WARN("Unknown tool type: {}", static_cast<int>(event.Tool));
	}
}