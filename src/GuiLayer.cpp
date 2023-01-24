#include "pch.hpp"
#include "GuiLayer.hpp"
#include "imgui.h"


void GuiLayer::OnUpdate()
{
    // Dear ImGui drawing here
    ImGui::Begin("Hello world");
    ImGui::Text("Some text...");
    ImGui::End();
}
