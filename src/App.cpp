#include "pch.hpp"
#include "App.hpp"
#include "Canvas/Canvas.hpp"
#include "rlgl.h"
#include "Input.hpp"
#include "Time.hpp"
#include "Window.hpp"
#include "Gui.hpp"
#include "Render/Renderer.hpp"


App* App::m_Instance = nullptr;

App::App(const AppConfig& appConfig) : m_AppConfig{appConfig}
{
	Logger::Init();
	Window::Init(appConfig.WindowWidth, appConfig.WindowHeight, appConfig.Name);
	Window::SetEventCallback(BIND_EVENT(App::OnEvent));
	Time::Init();
	Time::LockFPS(61.0);
	InitGui();

	m_Instance = this;
	m_Canvas = std::make_unique<Canvas>();
	m_ScriptEngine = std::make_unique<ScriptEngine>(*m_Canvas);
}

void App::Run()
{
	LOG_INFO("App started");
	while (IsRunning())
	{
		Input::ResetStates();
		Window::PollEventsOrWait();
		m_ScriptEngine->OnScriptsUpdate();
		m_Canvas->OnUpdate();

		Renderer::BeginFrame();
		{
			m_Canvas->Draw();
			DrawGui();
		}
		Renderer::EndFrame();
		Time::EndFrame();
	}

	LOG_INFO("App stopped");
	DestroyGui();
	Window::Close();
}

void App::Close()
{
	m_Running = false;
}

void App::RequestRedraw()
{
	glfwPostEmptyEvent();
}

bool App::IsRunning()
{
	return m_Running && !Window::ShouldClose();
}

void App::OnEvent(Event& event)
{
	Input::OnEvent(event);
}
