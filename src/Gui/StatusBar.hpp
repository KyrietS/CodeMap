#pragma once
#include "Gui/GuiElement.hpp"

namespace Gui
{
	class StatusBar : public GuiElement
	{
	public:
		void OnUpdate() override;
	};
}