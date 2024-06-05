#include "pch.hpp"
#include "Window.hpp"
#include "raylib.h"
#include "external/glfw/include/GLFW/glfw3.h"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"
#include "Time.hpp"


EventQueue* Window::s_EventQueue;
std::unique_ptr<Cursor> Window::s_MouseCursor;

void Window::Init(uint32_t width, uint32_t height, const std::string& title, EventQueue* eventQueue)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	InitWindow(width, height, title.c_str());
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);
	s_EventQueue = eventQueue;

	GLFWwindow* glfwWindow = glfwGetCurrentContext();
	s_MouseCursor = std::make_unique<Cursor>(glfwWindow);

	glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xPos, double yPos) {
		Events::Input::MouseMoved event((float)xPos, (float)yPos);
		Window::EventCallback(event);
	});
	glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods) {
		if (button < 0 || button > Mouse::LastCode)
		{
			LOG_WARN("MouseButtonCallback: unknown button - ignoring.");
			return;
		}

		MouseCode mouseCode = button;

		if (action == GLFW_PRESS)
		{
			Events::Input::MousePressed event(mouseCode);
			EventCallback(event);
		}
		if (action == GLFW_RELEASE)
		{
			Events::Input::MouseReleased event(mouseCode);
			EventCallback(event);
		}
	});
	glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
		Events::Input::MouseScrolled event((float)xOffset, (float)yOffset);
		EventCallback(event);
	});
	glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_UNKNOWN || key < 0 || key > Key::LastCode)
		{
			LOG_WARN("KeyCallback: unknown key - ignoring.");
			return;
		}

		KeyCode keyCode = key;

		if (action == GLFW_PRESS)
		{
			Events::Input::KeyPressed event(keyCode);
			EventCallback(event);
		}
		if (action == GLFW_REPEAT)
		{
			Events::Input::KeyPressed event(keyCode, true);
			EventCallback(event);
		}
		if (action == GLFW_RELEASE)
		{
			Events::Input::KeyReleased event(keyCode);
			EventCallback(event);
		}
	});
	glfwSetCharCallback(glfwWindow, [](GLFWwindow* window, unsigned int codePoint) {
		Events::Input::KeyTyped event((uint32_t)codePoint);
		EventCallback(event);
	});
}

uint32_t Window::GetWidth()
{
	return (uint32_t)GetScreenWidth();
}

uint32_t Window::GetHeight()
{
	return (uint32_t)GetScreenHeight();
}

Cursor& Window::GetMouseCursor()
{
	return *s_MouseCursor;
}

void Window::SetWindowTitle(const std::string& title)
{
	::SetWindowTitle(title.c_str());
}

void Window::PollEvents()
{
	glfwPollEvents(); // callbacks will be invoked here
}

void Window::PollEventsOrWait()
{
	// I need to pause frame-time to make sure that it is calculated correctly.
	// For example, if we waited 2 secods for an event, then calling GetFrameTime
	// would return 2 seconds and every time-frame-related code like Animation would break.
	// By using Pause and Resume I can subtraxt the time spent on waiting
	// So that user of Time::GetFrameTime is not aware of the waiting.
	Time::Pause();
	glfwWaitEvents();
	Time::Resume();
}

void Window::Close()
{
	rl_CloseWindow();
}

bool Window::ShouldClose()
{
	return WindowShouldClose();
}
