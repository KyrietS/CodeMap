#include "pch.hpp"
#include "Window.hpp"
#include "raylib.h"
#include "external/glfw/include/GLFW/glfw3.h"
#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Input.hpp"
#include "Time.hpp"


std::function<void(Event&)> Window::s_EventCallback;


void Window::Init(uint32_t width, uint32_t height, const std::string& title)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	InitWindow(width, height, title.c_str());
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	GLFWwindow* glfwWindow = glfwGetCurrentContext();

	glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xPos, double yPos) {
		MouseMovedEvent event((float)xPos, (float)yPos);
		Window::s_EventCallback(event);
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
			MousePressedEvent event(mouseCode);
			s_EventCallback(event);
		}
		if (action == GLFW_RELEASE)
		{
			MouseReleasedEvent event(mouseCode);
			s_EventCallback(event);
		}
	});
	glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		s_EventCallback(event);
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
			KeyPressedEvent event(keyCode);
			s_EventCallback(event);
		}
		if (action == GLFW_REPEAT)
		{
			KeyPressedEvent event(keyCode, true);
			s_EventCallback(event);
		}
		if (action == GLFW_RELEASE)
		{
			KeyReleasedEvent event(keyCode);
			s_EventCallback(event);
		}
	});
	glfwSetCharCallback(glfwWindow, [](GLFWwindow* window, unsigned int codePoint) {
		KeyTypedEvent event((uint32_t)codePoint);
		s_EventCallback(event);
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

void Window::SetEventCallback(std::function<void(Event&)> eventCallback)
{
	s_EventCallback = eventCallback;
}

void Window::Close()
{
	rl_CloseWindow();
}

bool Window::ShouldClose()
{
	return WindowShouldClose();
}
