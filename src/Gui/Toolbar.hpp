#pragma once

#include "Events/EventQueue.hpp"
#include "GuiElement.hpp"
#include "Canvas/Tool.hpp"

using TextureId = unsigned int;

namespace Gui
{

class Toolbar : public GuiElement
{
public:
	Toolbar(EventQueue&);
	void OnUpdate() override;

private:
	void ShowToolButton(ToolType tool);

	std::map<ToolType, std::shared_ptr<TextureId>> m_ToolIcons;
	ToolType m_SelectedTool = ToolType::None;
    EventQueue& m_EventQueue;
};
}