#pragma once
#include "Canvas/Canvas.hpp"
#include "ScriptEngine.hpp"
#include "Events/Event.hpp"
#include "Events/EventQueue.hpp"
#include "Layer.hpp"
#include "DearImGuiLayer.hpp"


struct AppConfig
{
	std::string Name = "CodeMap";
	std::string WorkingDir;
	int WindowWidth = 1000;
	int WindowHeight = 600;
};

class App
{
public:
	App(const AppConfig&);
	void Run();

private:
	void OnEvent(Event&);
	void Close();
	bool IsRunning();
	void FetchEvents();
	void ExecuteScripts();
	void UpdateLayers();
	void BeginFrame();
	void EndFrame();
	void DispatchEvents();
	void ReleaseResources();

	AppConfig m_AppConfig;
	bool m_Running = true;
	std::unique_ptr<Canvas> m_Canvas;
	std::unique_ptr<ScriptEngine> m_ScriptEngine;
	EventQueue m_EventQueue;

	DearImGuiLayer* m_DearImGuiLayer = nullptr;
	std::vector<std::unique_ptr<Layer>> m_Layers;

	static App* m_Instance;
};