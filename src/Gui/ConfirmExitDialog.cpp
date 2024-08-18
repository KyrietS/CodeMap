#include "pch.hpp"
#include "ConfirmExitDialog.hpp"
#include <imgui.h>
#include "Events/GuiEvents.hpp"
#include "Events/AppEvents.hpp"
#include "Events/EventDispatcher.hpp"

namespace Gui
{
	void ConfirmExitDialog::OnUpdate()
	{
		if (m_ShouldOpenPopup)
		{
			ImGui::OpenPopup("Exit?##confirm_exit_popup");
			m_ShouldOpenPopup = false;
			LOG_DEBUG("Opened ConfirmExit popup");
		}

		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Exit?##confirm_exit_popup", NULL, flags))
		{
			m_EventQueue.Push(EmptyEvent {}); // Opening popup in ImGui has fade animation, so keep updating the GUI
			ImGui::Text("Are you sure you want to exit?");

			if (ImGui::Button("Exit", ImVec2(120, 0))) 
			{ 
				ImGui::CloseCurrentPopup(); 
				m_EventQueue.Push(Events::App::Quit{});
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) 
			{ 
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::EndPopup();
		}
	}

	void ConfirmExitDialog::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Gui::ShowConfirmExitDialog>(BIND_EVENT(ConfirmExitDialog::OnShowConfirmExitDialogEvent));
	}

	bool ConfirmExitDialog::OnShowConfirmExitDialogEvent(const Events::Gui::ShowConfirmExitDialog& event)
	{
		m_ShouldOpenPopup = true;
		return true;
	}
}