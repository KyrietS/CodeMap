#include "pch.hpp"
#include "GuiLayer.hpp"
#include "Canvas/Deserializer/CanvasDeserializer.hpp"
#include "Assets/AssetLoader.hpp"
#include "Events/AppEvents.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"
#include "Timer.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Entity.hpp"
#include "App.hpp"
#include "Input.hpp"
#include "entt/entt.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>


namespace
{
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
}

// TODO: Move to separate file and class
void GuiLayer::ShowToolbar()
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
	ImVec2 overlayPos = { workPos.x + padding, workPos.y + padding * 2 };
	ImVec2 positionPivot = { 0.0f, 0.0f }; 	// Left-Top corner
	ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always, positionPivot);
	ImGui::SetNextWindowViewport(viewport->ID);

	enum class Tool
	{
		None,
		Hand,
		Select,
		Arrow,
		Highlight
	};

	static Tool selectedTool = Tool::None;

	if (ImGui::Begin("Toolbar", nullptr, window_flags))
	{
		Tool tools[] = {Tool::Hand, Tool::Select, Tool::Arrow, Tool::Highlight};

		float height = 32.0f; // Icon size
		std::uintptr_t textureId = *m_IconPlaceholder;
		auto buttonSelectedColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

		for (auto tool : tools)
		{
			ImGui::PushID((int)tool);
			if (tool == selectedTool)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, buttonSelectedColor);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent background
			}
			if (ImGui::ImageButton("tool_button", (void*)textureId, ImVec2(height, height)))
			{
				selectedTool = tool; // TODO: Send event that tool has changed
			}
			ImGui::PopStyleColor();
			ImGui::PopID();
		}
	}
	ImGui::End();
}

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

void GuiLayer::SaveCanvasToFile(const std::string& filename)
{
	LOG_DEBUG("Saving canvas to: {}", filename);
	m_EventQueue.Push(Events::Canvas::SaveToFile{ filename });
}

void GuiLayer::LoadCanvasFromFile(const std::string& filename)
{
	LOG_DEBUG("Loading canvas from: {}", filename);
	m_EventQueue.Push(Events::Canvas::LoadFromFile{ filename });
}

void GuiLayer::ShowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				LoadCanvasFromFile("test.svg");
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) 
			{
				SaveCanvasToFile("canvas.xml");
			}
			if (ImGui::MenuItem("Save As..")) {}
			if (ImGui::MenuItem("Quit", "Alt+F4")) 
			{
				m_EventQueue.Push(Events::App::Quit{});
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {
                m_EventQueue.Push(Events::Canvas::Undo{});
            }
			if (ImGui::MenuItem("Redo", "CTRL+Y"))
            {
                m_EventQueue.Push(Events::Canvas::Redo{});
            }
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void GuiLayer::ShowProperties()
{
	if (!m_SelectedEntity || !m_SelectedEntity->IsValid())
		return;

	if (m_SelectedEntity->HasComponent<Components::Transform>())
	{
		ImGui::Begin("Transform");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Transform>());
		ImGui::End();
	}
	if (m_SelectedEntity->HasComponent<Components::Text>())
	{
		ImGui::Begin("Text");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Text>());
		ImGui::End();
	}
	if (m_SelectedEntity->HasComponent<Components::Arrow>())
	{
		ImGui::Begin("Arrow");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Arrow>());
		ImGui::End();
	}
	if (m_SelectedEntity->HasComponent<Components::Highlight>())
	{
		ImGui::Begin("Highlight");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Highlight>());
		ImGui::End();
	}
}

void GuiLayer::ShowPropertiesFor(Components::Transform& transform)
{
	ImGui::Text("Transform");
	ImGui::Separator();
	ImGui::DragFloat2("Position", &transform.Translation[0], 1.0f, 0.0f, 0.0f, "%.0f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
	ImGui::InputInt("Index", &transform.Index, 1, 10);
}

void GuiLayer::ShowPropertiesFor(Components::Text& text)
{
	ImGui::Text("Text");
	ImGui::Separator();
	std::string utf8Content = text.GetContentInUtf8();
	ImGui::InputTextMultiline("Content", &utf8Content);
	text.SetUtf8Content(utf8Content);
	ImGui::DragFloat("Font size", &text.FontSize, 1.0f, 4.0f, 256.0f, "%.0f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

	ImGui::BeginDisabled();
	ImGui::ColorEdit4("Font color", &text.FontColor[0], ImGuiColorEditFlags_NoAlpha);
	ImGui::EndDisabled();
}

void GuiLayer::ShowPropertiesFor(Components::Arrow& arrow)
{
	ImGui::Text("Arrow");
	ImGui::Separator();
	ImGui::ColorPicker3("Arrow color", &arrow.StrokeColor[0], 
		ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);
	ImGui::ColorEdit3("Arrowhead", &arrow.ArrowheadColor[0], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayHex);
	ImGui::DragFloat("Thickness", &arrow.Thickness, 0.5f, 0.1f, 256.0f, "%.1f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
}

void GuiLayer::ShowPropertiesFor(Components::Highlight& highlight)
{
	ImGui::Text("Highlight");
	ImGui::Separator();
	ImGui::ColorPicker4("Highlight color", &highlight.Color[0],
		ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);

	const char* items[] = { "Alpha", "Multiply" };
	int currentOption = static_cast<int>(highlight.BlendMode);
	ImGui::Combo("Blending", &currentOption, items, IM_ARRAYSIZE(items));
	highlight.BlendMode = static_cast<Render::BlendMode>(currentOption);

	ImGui::SameLine(); HelpMarker("Set the blending mode for Highligh object.");
}

void SetupDockSpace(ImGuiID viewportDockSpaceId)
{
	static bool firstTime = true;
	if (!firstTime)
		return;

	ImGuiID dockRight = ImGui::DockBuilderSplitNode(viewportDockSpaceId, ImGuiDir_Right, 0.3f, nullptr, &viewportDockSpaceId);
	ImGui::DockBuilderDockWindow("Transform", dockRight);
	ImGui::DockBuilderDockWindow("Arrow", dockRight);
	ImGui::DockBuilderDockWindow("Text", dockRight);
	ImGui::DockBuilderDockWindow("Highlight", dockRight);

	firstTime = false;
}

GuiLayer::GuiLayer(EventQueue& eventQueue)
	: m_EventQueue(eventQueue) 
{
	// TODO: This is a placeholder icon.
	Assets::Icon icon = Assets::LoadNoIconRGBA();
	m_IconPlaceholder = Renderer::LoadTextureFromBytes(icon.RGBA, icon.Width, icon.Height);
}

void GuiLayer::OnUpdate()
{
	ShowMainMenuBar();

	static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None 
		| ImGuiDockNodeFlags_PassthruCentralNode
		| ImGuiDockNodeFlags_NoDockingInCentralNode;
	ImGuiID viewportDockSpaceId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockFlags);
	SetupDockSpace(viewportDockSpaceId);

	ShowToolbar();
	ShowProperties();
	ShowMetaInfoOverlay();
	ShowMousePositionOverlay();

	if (ImGui::BeginPopupContextItem("popup_id"))
	{
		ImGui::Text("Example popup");
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}

void GuiLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Gui::ShowPopup>(BIND_EVENT(GuiLayer::OnShowPopupEvent));
	dispatcher.Dispatch<Events::Gui::ShowProperties>(BIND_EVENT(GuiLayer::OnShowPropertiesEvent));
}

void GuiLayer::OnShowPropertiesEvent(const Events::Gui::ShowProperties& event)
{
	m_SelectedEntity = event.Entity ? std::make_unique<Entity>(event.Entity) : nullptr;
}

void GuiLayer::OnShowPopupEvent(const Events::Gui::ShowPopup& event)
{
	LOG_DEBUG("ShowPopup");
	ImGui::OpenPopup("popup_id");
}
