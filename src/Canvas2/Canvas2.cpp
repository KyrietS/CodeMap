#include "Canvas2.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Window.hpp"
#include "Controllers/CameraController.hpp"
#include "Controllers/ToolboxController.hpp"
#include "Controllers/PasteImageController.hpp"
#include "Elements/ArrowElement.hpp"
#include "Canvas/Serializer/SVG/SvgSerializer2.hpp"
#include "Canvas/Deserializer/SVG/SvgDeserializer2.hpp"
#include <ranges>

Canvas2::Canvas2(EventQueue& eventQueue)
	: m_EventQueue(eventQueue)
{
	m_Controllers.push_back(std::make_unique<Controllers::CameraController>(m_Camera));
	m_Controllers.push_back(std::make_unique<Controllers::ToolboxController>(m_Camera, m_EventQueue, m_Elements));
	m_Controllers.push_back(std::make_unique<Controllers::PasteImageController>(m_Camera, m_EventQueue, m_Elements));
}

void Canvas2::Draw()
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

void Canvas2::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Handle<Events::Canvas::SaveToFile>(BIND_EVENT(OnCanvasSaveToFile));
	dispatcher.Handle<Events::Canvas::LoadFromFile>(BIND_EVENT(OnCanvasLoadFromFile));

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

bool Canvas2::OnCanvasSaveToFile(const Events::Canvas::SaveToFile& event)
{
	LOG_DEBUG("[EVENT] Canvas received SaveToFile event with path: {}", event.Filename);
	SvgSerializer2 { m_Elements }.Serialize();
	return true;
}

bool Canvas2::OnCanvasLoadFromFile(const Events::Canvas::LoadFromFile& event)
{
	LOG_DEBUG("[EVENT] Canvas received LoadFromFile event with path: {}", event.Filename);
	SvgDeserializer2 { m_Camera, m_Elements, m_EventQueue }.Deserialize(event.Filename);
	return true;
}

void Canvas2::DrawGrid()
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