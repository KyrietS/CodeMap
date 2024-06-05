#pragma once

#include "Events/EventQueue.hpp"
#include "GuiElement.hpp"
#include "Canvas2/Tool.hpp"

using TextureId = unsigned int;

namespace Gui
{

class Toolbar : public GuiElement
{
public:
	Toolbar(EventQueue&);
	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void ShowToolbar();
	void ShowToolbarButton(ToolType tool, float width, float height);
	void ShowToolbox();
	void ShowToolboxButton(ToolType tool, float width, float height);

	void* GetToolIcon(ToolType tool);

	std::map<ToolType, std::shared_ptr<TextureId>> m_ToolIcons;
	ToolType m_SelectedTool = ToolType::None;
    EventQueue& m_EventQueue;
	std::uint64_t frameNumber = 0; // TODO: Move to GuiContext
	bool toolboxVisible = true; // TODO: Move to GuiContext
};
}