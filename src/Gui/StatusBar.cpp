#include "pch.hpp"
#include "StatusBar.hpp"
#include <imgui.h>
#include "Canvas/Canvas.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include <imgui_internal.h>

namespace Gui
{
	void StatusBar::OnUpdate()
	{
		float height = ImGui::GetFrameHeight();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		if( ImGui::BeginViewportSideBar( "StatusBar", nullptr, ImGuiDir_Down, height, flags ) )
		{
			ImGui::Text( "Status Bar" );
			ImGui::End();
		}
	}
}