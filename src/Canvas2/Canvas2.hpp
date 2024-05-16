#pragma once
#include "Events/EventQueue.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Controllers/IController.hpp"

class Canvas2
{
public:
	Canvas2(EventQueue&);

	void Draw();
	void OnEvent(Event&);

private:
	void DrawGrid();

	std::vector<std::unique_ptr<Controllers::IController>> m_Controllers;


	EventQueue& m_EventQueue;
	CanvasCamera m_Camera;
};