#pragma once
#include "Canvas/Canvas.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Events/Event.hpp"
#include "Events/EventQueue.hpp"
#include "Events/AppEvents.hpp"
#include "Layer.hpp"
#include "DearImGuiLayer.hpp"


struct AppConfig
{
	std::string AppName = "CodeMap";
	std::string ProjectName = "New project";
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
	void OnQuit(const Events::App::Quit&);
	bool OnProjectSaved(const Events::App::ProjectSaved&);
	bool OnProjectUnsaved(const Events::App::ProjectUnsaved&);

	bool IsRunning();
	void FetchEvents();
	void UpdateLayers();
	void BeginFrame();
	void EndFrame();
	void DispatchEvents();
	void ReleaseResources();

	AppConfig m_AppConfig;
	bool m_Running = true;

	CanvasElements m_CanvasElements;
	EventQueue m_EventQueue;

	DearImGuiLayer* m_DearImGuiLayer = nullptr;
	std::vector<std::unique_ptr<Layer>> m_Layers;

	static App* m_Instance;
};