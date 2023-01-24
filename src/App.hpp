#pragma once
#include "Canvas/Canvas.hpp"
#include "ScriptEngine.hpp"
#include "Events/Event.hpp"
#include "Layer.hpp"
#include "DearImGuiLayer.hpp"


struct AppConfig
{
	std::string Name = "CodeMap";
	std::string WorkingDir;
	int WindowWidth = 800;
	int WindowHeight = 600;
};

class App
{
public:
	App(const AppConfig&);
	void Run();
	void Close();
	void OnEvent(Event&);

	Canvas* GetCanvas() { return m_Canvas.get(); }

	static App& Get() { return *m_Instance; }
	static void RequestRedraw();
private:
	bool IsRunning();
	void FetchEvents();
	void ExecuteScripts();
	void UpdateLayers();
	void BeginFrame();
	void EndFrame();
	void ReleaseResources();

	AppConfig m_AppConfig;
	bool m_Running = true;
	std::unique_ptr<Canvas> m_Canvas;
	std::unique_ptr<ScriptEngine> m_ScriptEngine;

	DearImGuiLayer* m_DearImGuiLayer = nullptr;
	std::vector<std::unique_ptr<Layer>> m_Layers;

	static App* m_Instance;
};