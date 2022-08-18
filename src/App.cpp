#include "pch.hpp"
#include "App.hpp"
#include "raylib.h"
#include "Canvas/Canvas.hpp"
#include "rlgl.h"


App* App::m_Instance = nullptr;

App::App(const AppConfig& appConfig) : m_AppConfig{appConfig}
{
	Logger::Init();

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(m_AppConfig.WindowWidth, m_AppConfig.WindowHeight, m_AppConfig.Name.c_str());
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);
	m_Instance = this;
	m_Canvas = std::make_unique<Canvas>();
	m_ScriptEngine = std::make_unique<ScriptEngine>(*m_Canvas);
}

void App::Run()
{
	LOG_INFO("App started");
	while (IsRunning())
	{
		m_ScriptEngine->OnScriptsUpdate();
		m_Canvas->OnUpdate();
		m_Canvas->Draw();
	}

	LOG_INFO("App stopped");
	CloseWindow();
}

void App::Close()
{
	m_Running = true;
}

bool App::IsRunning()
{
	return m_Running && !WindowShouldClose();
}
