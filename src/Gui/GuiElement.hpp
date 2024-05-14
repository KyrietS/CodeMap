#pragma once

class Event;

using ImGuiID = unsigned int;

namespace Gui
{
class GuiElement
{
public:
	GuiElement() = default;
	virtual ~GuiElement() = default;
	virtual void OnUpdate() = 0;
	virtual void OnEvent(Event&) {};
};
}