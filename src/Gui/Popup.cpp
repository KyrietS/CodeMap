#include "pch.hpp"
#include "Popup.hpp"
#include <imgui.h>
#include "Events/GuiEvents.hpp"
#include "Events/EventDispatcher.hpp"

namespace Gui
{
void Popup::OnUpdate()
{
	if (ImGui::BeginPopupContextItem("popup_id"))
	{
		ImGui::Text("Example popup");
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}

void Popup::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Gui::ShowPopup>(BIND_EVENT(Popup::OnShowPopupEvent));
}

void Popup::OnShowPopupEvent(const Events::Gui::ShowPopup& event)
{
	LOG_DEBUG("ShowPopup");
	ImGui::OpenPopup("popup_id");
}

}