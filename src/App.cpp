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
#include "Messages.hpp"


App* App::m_Instance = nullptr;

App::App(const AppConfig& appConfig) : m_AppConfig{appConfig}
{
	Logger::Init();
	Window::Init(appConfig.WindowWidth, appConfig.WindowHeight, appConfig.Name);
	Window::SetEventCallback(BIND_EVENT(App::OnEvent));
	Time::Init();
	Time::LockFPS(61.0);

	m_Instance = this;
	m_Canvas = std::make_unique<Canvas>(m_Dispatcher);
	m_ScriptEngine = std::make_unique<ScriptEngine>(*m_Canvas);

	m_Layers.push_back(std::make_unique<CanvasLayer>(*m_Canvas));
	m_Layers.push_back(std::make_unique<GuiLayer>(m_Dispatcher));

	auto dearImGuiLayer = std::make_unique<DearImGuiLayer>();
	m_DearImGuiLayer = dearImGuiLayer.get();
	m_Layers.push_back(std::move(dearImGuiLayer));

    RegisterMessageHandlers();
}

void App::Run()
{
	LOG_INFO("App started");
	while (IsRunning())
	{
		FetchEvents();
		ExecuteScripts();

		BeginFrame();
		UpdateLayers();
		EndFrame();

        ProcessMessages();
	}

	LOG_INFO("App stopped");
	ReleaseResources();
}

void App::Close()
{
	m_Running = false;
}

void App::RequestRedraw()
{
	glfwPostEmptyEvent();
}

void App::FetchEvents()
{
	Input::ResetStates();
	Window::PollEventsOrWait();
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

void App::RegisterMessageHandlers()
{
    m_Dispatcher.listen<Messages::App::Quit>([this](const auto& msg) {
        Close();
    });
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

void App::ProcessMessages()
{
    m_Dispatcher.process();
}

void App::ReleaseResources()
{
	m_Layers.clear(); // remove all layers and free ImGui Context
	Window::Close();
}

void App::OnEvent(Event& event)
{
	for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
	{
		if (event.Handled)
			break;
		(*it)->OnEvent(event);
	}
}
