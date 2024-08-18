#pragma once

#include "GuiElement.hpp"
#include "Events/GuiEvents.hpp"
#include "Events/EventQueue.hpp"

namespace Gui
{
	class ConfirmExitDialog : public GuiElement
	{
	public:
		ConfirmExitDialog(EventQueue& eventQueue) : m_EventQueue(eventQueue) {}
		void OnUpdate() override;
		void OnEvent(Event&) override;

	private:
		bool OnShowConfirmExitDialogEvent(const Events::Gui::ShowConfirmExitDialog&);

		bool m_ShouldOpenPopup = false;
		EventQueue& m_EventQueue;
	};
}