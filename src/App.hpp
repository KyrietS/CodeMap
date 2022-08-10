#pragma once
#include "Canvas.hpp"


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
	Canvas* GetCanvas() { return m_Canvas.get(); }

	static App& Get() { return *m_Instance; }
private:
	bool IsRunning();

	AppConfig m_AppConfig;
	bool m_Running = true;
	std::unique_ptr<Canvas> m_Canvas;

	static App* m_Instance;
};