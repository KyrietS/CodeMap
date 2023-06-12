#include "pch.hpp"
#include "GuiLayer.hpp"
#include "imgui.h"
#include "Timer.hpp"
#include "Canvas/Canvas.hpp"
#include "App.hpp"
#include "Input.hpp"

void ShowMetaInfoOverlay()
{
    ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_NoDecoration 
        | ImGuiWindowFlags_NoDocking 
        | ImGuiWindowFlags_AlwaysAutoResize 
        | ImGuiWindowFlags_NoSavedSettings 
        | ImGuiWindowFlags_NoFocusOnAppearing 
        | ImGuiWindowFlags_NoNav 
        | ImGuiWindowFlags_NoMove;

    const float padding = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 workPos = viewport->WorkPos; // Position of Top-Left corner of working area (without menubar)
    ImVec2 workSize = viewport->WorkSize;
    ImVec2 overlayPos = { padding, workPos.y + workSize.y - padding };
    ImVec2 positionPivot = { 0.0f, 1.0f }; // Left-Bottom corner
    ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always, positionPivot);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("MetaInfoOverlay", nullptr, window_flags))
    {
        int zoomLevel = (int)(Canvas::Camera().GetZoom() * 100);
        int fps = (int)Time::GetFPS();

        ImGui::Text("zoom: %d%%", zoomLevel);
        ImGui::Separator();
        ImGui::Text("%d FPS", fps);
    }
    ImGui::End();
}

void ShowMousePositionOverlay()
{
    // Show mouse position in the Right-Bottom corner
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoNav
        | ImGuiWindowFlags_NoMove;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    const float padding = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 workPos = viewport->WorkPos; // Position of Top-Left corner of working area (without menubar)
    ImVec2 workSize = viewport->WorkSize;
    ImVec2 overlayPos = { workPos.x + workSize.x - padding, workPos.y + workSize.y - padding };
    ImVec2 positionPivot = { 1.0f, 1.0f }; // Right-Bottom corner
    ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always, positionPivot);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("MousePositionOverlay", nullptr, window_flags))
    {
        auto mousePos = Input::GetWorldMousePosition();
        ImGui::Text("(%.0f, %.0f)", mousePos.x, mousePos.y);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void GuiLayer::SaveCanvasToFile(std::string_view filename)
{
    LOG_DEBUG("Saving canvas to: {}", filename);
    LOG_DEBUG("Serialized: {}", m_SvgSerializer->Serialize());
    // TODO: save serializer output to a file
}

void GuiLayer::ShowMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) 
            {
                SaveCanvasToFile("canvas.xml");
            }
            if (ImGui::MenuItem("Save As..")) {}
            if (ImGui::MenuItem("Quit", "Alt+F4")) 
            {
                App::Get().Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

GuiLayer::GuiLayer(std::unique_ptr<CanvasSerializer> serializer)
    : m_SvgSerializer(std::move(serializer)) {}

void GuiLayer::OnUpdate()
{
    ShowMainMenuBar();

    ShowMetaInfoOverlay();
    ShowMousePositionOverlay();
}
