#include "pch.hpp"
#include "App.hpp"
#include "Canvas/Canvas.hpp"
#include "rlgl.h"
#include "Input.hpp"
#include "Time.hpp"
#include "Window.hpp"
#include "GuiLayer.hpp"
#include "Render/Renderer.hpp"
#include "CanvasLayer.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/AppEvents.hpp"


App* App::m_Instance = nullptr;

App::App(const AppConfig& appConfig) : m_AppConfig{appConfig}
{
	Logger::Init();
	Window::Init(appConfig.WindowWidth, appConfig.WindowHeight, appConfig.Name, &m_EventQueue);
	Time::Init();
	Time::LockFPS(61.0);

	m_Instance = this;
	m_Canvas = std::make_unique<Canvas>(m_EventQueue);
	m_ScriptEngine = std::make_unique<ScriptEngine>(*m_Canvas);

	m_Layers.push_back(std::make_unique<CanvasLayer>(*m_Canvas));
	m_Layers.push_back(std::make_unique<GuiLayer>(m_EventQueue));

	auto dearImGuiLayer = std::make_unique<DearImGuiLayer>();
	m_DearImGuiLayer = dearImGuiLayer.get();
	m_Layers.push_back(std::move(dearImGuiLayer));
}

void App::Run()
{
	LOG_INFO("App started");
	while (IsRunning())
	{
		FetchEvents();
		DispatchEvents();
		ExecuteScripts();

		BeginFrame();
		UpdateLayers();
		EndFrame();
	}

	LOG_INFO("App stopped");
	ReleaseResources();
}

void App::Close()
{
	m_Running = false;
}

void App::FetchEvents()
{
	Input::ResetStates();
	if (m_EventQueue.Empty())
		Window::PollEventsOrWait();
	else
		Window::PollEvents();
}

void App::ExecuteScripts()
{
	m_ScriptEngine->OnScriptsUpdate();
}

void App::UpdateLayers()
{
	for (auto& layer : m_Layers)
	{
		layer->OnUpdate();
	}
}

bool App::IsRunning()
{
	return m_Running && !Window::ShouldClose();
}

void App::BeginFrame()
{
	Renderer::BeginFrame();
	m_DearImGuiLayer->Begin();
}

void App::EndFrame()
{
	m_DearImGuiLayer->End();
	Renderer::EndFrame();
	Time::EndFrame();
}

void App::DispatchEvents()
{
	while(not m_EventQueue.Empty())
	{
		auto event = m_EventQueue.Pop();
		OnEvent(event);
	}
}

void App::ReleaseResources()
{
	m_Layers.clear(); // remove all layers and free ImGui Context
	Window::Close();
}

void App::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::App::Quit>([this](const auto&){ Close(); });

	for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
	{
		if (event.Handled)
			break;
		(*it)->OnEvent(event);
	}
}
