#include "pch.hpp"
#include "App.hpp"
#include "Canvas/Canvas.hpp"
#include "rlgl.h"
#include "Input.hpp"
#include "Time.hpp"
#include "Window.hpp"


App* App::m_Instance = nullptr;

App::App(const AppConfig& appConfig) : m_AppConfig{appConfig}
{
	Logger::Init();
	Window::Init(appConfig.WindowWidth, appConfig.WindowHeight, appConfig.Name);
	Input::Init();
	Time::Init();
	Time::LockFPS(61.0);

	m_Instance = this;
	m_Canvas = std::make_unique<Canvas>();
	m_ScriptEngine = std::make_unique<ScriptEngine>(*m_Canvas);
}

void App::Run()
{
	LOG_INFO("App started");
	while (IsRunning())
	{
		Input::PollEventsOrWait();
		m_ScriptEngine->OnScriptsUpdate();
		// Rework: For now it's ok, to call OnUpdate() and Draw() here, but at some point
		// I want to register the Canvas object in some EventDispatcher and send events
		// from lower layers: RedrawEvent, AnimationTickEvent, KeyPressedEvent, etc.
		// This way I can achieve event-based application and stop writing code that is
		// executed every frame in OnUpdate() function, which is nice, but not for GUI
		// apps like this one.
		m_Canvas->OnUpdate();
		m_Canvas->Draw();

		Time::EndFrame();
	}

	LOG_INFO("App stopped");
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
