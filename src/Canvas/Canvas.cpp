#include "Canvas.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/AppEvents.hpp"
#include "Window.hpp"
#include "Controllers/CameraController.hpp"
#include "Controllers/SelectionController.hpp"
#include "Controllers/ToolboxController.hpp"
#include "Controllers/PasteImageController.hpp"
#include "Controllers/UndoRedoController.hpp"
#include "Controllers/CommonKeyboardShortcutsController.hpp"
#include "Elements/ArrowElement.hpp"
#include "Canvas/Serializer/SVG/SvgSerializer.hpp"
#include "Canvas/Deserializer/SVG/SvgDeserializer.hpp"
#include "Dialogs.hpp"
#include <ranges>

namespace
{
	std::string LoadFileContent(const std::filesystem::path& filename)
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			LOG_ERROR("Failed to open file: {0}", filename);
			return "";
		}

		std::stringstream ss;
		ss << file.rdbuf();
		file.close();

		return ss.str();
	}
}

Canvas::Canvas(CanvasElements& elements, EventQueue& eventQueue)
	: m_Elements(elements), m_EventQueue(eventQueue)
{
	m_Controllers.push_back(std::make_unique<Controllers::CommonKeyboardShortcutsController>(m_EventQueue));
	m_Controllers.push_back(std::make_unique<Controllers::CameraController>(m_Camera, m_EventQueue));
	m_Controllers.push_back(std::make_unique<Controllers::SelectionController>(m_Camera, m_EventQueue, m_Elements));
	m_Controllers.push_back(std::make_unique<Controllers::ToolboxController>(m_Camera, m_EventQueue, m_Elements));
	m_Controllers.push_back(std::make_unique<Controllers::PasteImageController>(m_Camera, m_EventQueue, m_Elements));
	m_Controllers.push_back(std::make_unique<Controllers::UndoRedoController>(m_EventQueue, m_Elements));
}

void Canvas::Draw()
{
	Renderer::ClearScreen(VColor::RayWhite);
	Renderer::BeginCameraView(m_Camera.GetData());
	{
		DrawGrid();

		for (auto& [id, element] : m_Elements | std::views::reverse)
		{
			element->Draw();
		}

		for (auto& controller : m_Controllers)
		{
			controller->Draw();
		}
	}
	Renderer::EndCameraView();
}

void Canvas::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Handle<Events::Canvas::SaveToFile>(BIND_EVENT(OnCanvasSaveToFile));
	dispatcher.Handle<Events::Canvas::LoadFromFile>(BIND_EVENT(OnCanvasLoadFromFile));
	dispatcher.Dispatch<Events::Canvas::MakeSnapshot>(BIND_EVENT(OnMakeSnapshot));

	for (auto& controller : m_Controllers)
	{
		if (event.Handled)
			return;
		controller->OnEvent(event);
	}

	for (auto& [id, element] : m_Elements)
	{
		if (event.Handled)
			return;
		element->OnEvent(event);
	}
}

bool Canvas::OnCanvasSaveToFile(const Events::Canvas::SaveToFile& event)
{
	if (not m_CurrentFilePath.has_value() or event.SaveAs)
	{
		if (auto path = Dialogs::SaveFileAsDialog())
		{
			SaveFileAs(*path);
		}
	}
	else
	{
		SaveFileAs(m_CurrentFilePath.value());
	}
	
	if (m_CurrentFilePath.has_value())
	{
		m_IsSaved = true;
		m_EventQueue.Push(Events::App::ProjectSaved { m_CurrentFilePath.value().string() });
	}

	return true;
}

bool Canvas::OnCanvasLoadFromFile(const Events::Canvas::LoadFromFile& event)
{
	if (not m_IsSaved)
	{
		auto answer = Dialogs::MessageBoxDialog(
			"Open new file",
			"Current project is not saved.\nAre you sure you want to open a new file?",
			Dialogs::Type::YesNo,
			Dialogs::Icon::Warning,
			Dialogs::Result::Reject
		);
		if (answer != Dialogs::Result::Accept)
		{
			return true;
		}
	}

	if (auto path = Dialogs::OpenFileDialog())
	{
		if (not path->extension().string().ends_with(".svg"))
		{
			auto answer = Dialogs::MessageBoxDialog(
				"Invalid file format",
				"Selected file is not an SVG file.\nLoad it anyway?",
				Dialogs::Type::YesNoCancel,
				Dialogs::Icon::Question
			);
			if (answer != Dialogs::Result::Accept)
			{
				return true;
			}
		}

		std::string fileContent = LoadFileContent(*path);
		SvgDeserializer { m_Camera, m_Elements, m_EventQueue }.Deserialize(fileContent);
		m_CurrentFilePath = *path;
		m_IsSaved = true;
		m_EventQueue.Push(Events::App::ProjectSaved { m_CurrentFilePath.value().string() });
	}
	
	return true;
}

void Canvas::OnMakeSnapshot(const Events::Canvas::MakeSnapshot&)
{
	m_IsSaved = false;
}

void Canvas::SaveFileAs(const std::filesystem::path& path)
{
	LOG_INFO("Saving to file: {}", path);
	const std::string svgContent = SvgSerializer { m_Elements }.Serialize();
	if (std::ofstream file(path); file.is_open())
	{
		file << svgContent;
	}
	else
	{
		LOG_ERROR("Failed to save file: {}", path);
		return;
	}

	m_CurrentFilePath = path;
	m_EventQueue.Push(Events::App::ProjectSaved { path.string() });
}

void Canvas::DrawGrid()
{
	const float zoom = m_Camera.GetZoom();
	const int DOT_GAP_SIZE = 50;
	const int NUM_OF_DOTS_HORIZONTAL = static_cast<int>((Window::GetWidth() / zoom) / DOT_GAP_SIZE + 2);
	const int NUM_OF_DOTS_VERTICAL = static_cast<int>((Window::GetHeight() / zoom) / DOT_GAP_SIZE + 2);
	float DOT_SIZE = 4;
	if (zoom < 1.0f)
	{
		DOT_SIZE *= (2.0f - zoom);
	}

	glm::vec2 dotScreenPos = { 0, 0 };
	glm::vec2 dotWorldPos = m_Camera.GetScreenToWorld(dotScreenPos);
	glm::vec2 firstDotScreenPos = {
		DOT_GAP_SIZE * ceil(dotWorldPos.x / DOT_GAP_SIZE) - DOT_GAP_SIZE,
		DOT_GAP_SIZE * ceil(dotWorldPos.y / DOT_GAP_SIZE) - DOT_GAP_SIZE
	};

	for (int i = 0; i < NUM_OF_DOTS_VERTICAL; i++)
	{
		float dotY = firstDotScreenPos.y + (i * DOT_GAP_SIZE);
		for (int k = 0; k < NUM_OF_DOTS_HORIZONTAL; k++)
		{
			float dotX = firstDotScreenPos.x + (k * DOT_GAP_SIZE);
			Renderer::DrawRectangle({ dotX, dotY }, DOT_SIZE, DOT_SIZE, VColor::LightGray);
		}
	}
}