#include "pch.hpp"
#include "App.hpp"
#include "raylib.h"
#include "Canvas.hpp"
#include "rlgl.h"

App* App::m_Instance = nullptr;

App::App(const AppConfig& appConfig) : m_AppConfig{appConfig}
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(m_AppConfig.WindowWidth, m_AppConfig.WindowHeight, m_AppConfig.Name.c_str());
	SetTargetFPS(60);
	m_Instance = this;
	m_Canvas = std::make_unique<Canvas>();
}

void App::Run()
{
	while (IsRunning())
	{
		m_Canvas->OnUpdate();
		m_Canvas->Draw();
	}

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
