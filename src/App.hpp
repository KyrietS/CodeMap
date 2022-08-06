#pragma once
#include "Canvas.hpp"
#include <string>
#include <memory>


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

	static App& Get() { return *m_Instance; }
private:
	bool IsRunning();

	AppConfig m_AppConfig;
	bool m_Running = true;
	std::unique_ptr<Canvas> m_Canvas;

	static App* m_Instance;
};