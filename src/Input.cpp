#include "pch.hpp"
#include "Input.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Canvas/Canvas.hpp"
#include "Time.hpp"


GLFWwindow* Input::s_GlfwWindow;
std::array<Input::InputState, GLFW_MOUSE_BUTTON_LAST+1> Input::s_MouseState;
std::array<Input::InputState, GLFW_KEY_LAST + 1> Input::s_KeyState;


Vector2 Input::s_MousePosition;
Vector2 Input::s_LastMousePos;
Vector2 Input::s_MouseScroll;

bool Input::s_IsTextMode = false;
std::queue<KeyCode> Input::s_KeyQueue;
std::queue<uint32_t> Input::s_CharQueue;

void Input::Init()
{
	// Set GLFW callbacks
	s_GlfwWindow = glfwGetCurrentContext();

	glfwSetCursorPosCallback(s_GlfwWindow, Input::MousePositionCallback);
	glfwSetMouseButtonCallback(s_GlfwWindow, Input::MouseButtonCallback);
	glfwSetScrollCallback(s_GlfwWindow, Input::MouseScrollCallback);
	glfwSetKeyCallback(s_GlfwWindow, Input::KeyCallback);
	glfwSetCharCallback(s_GlfwWindow, Input::CharCallback);

	s_LastMousePos = { 0.0f, 0.0f };
}

void Input::PollEvents()
{
	ResetStates();
	glfwPollEvents(); // callbacks will be invoked here
}

void Input::PollEventsOrWait()
{
	// I need to pause frame-time to make sure that it is calculated correctly.
	// For example, if we waited 2 secods for an event, then calling GetFrameTime 
	// would return 2 seconds and every time-frame-related code like Animation would break.
	// By using Pause and Resume I can subtraxt the time spent on waiting
	// So that user of Time::GetFrameTime is not aware of the waiting.

	ResetStates();

	Time::Pause();
	glfwWaitEvents();
	Time::Resume();
}

Vector2 Input::GetScreenMousePosition()
{
	return s_MousePosition;
}

Vector2 Input::GetWorldMousePosition()
{
	return Canvas::Camera().GetScreenToWorld(s_MousePosition);
}

Vector2 Input::GetWorldMousePositionRelativeTo(Vector2 origin)
{
	return Vector2Subtract(GetWorldMousePosition(), origin);
}

Vector2 Input::GetMouseDelta()
{
	return Vector2Subtract(s_MousePosition, s_LastMousePos);
}

Vector2 Input::GetMouseWheelMove()
{
	return s_MouseScroll;
}

bool Input::IsMouseButtonPressed(MouseCode mouseCode)
{
	return s_MouseState[mouseCode].IsPressed;
}

bool Input::IsMouseButtonDown(MouseCode mouseCode)
{
	return s_MouseState[mouseCode].IsDown;
}

bool Input::IsMouseButtonReleased(MouseCode mouseCode)
{
	return s_MouseState[mouseCode].IsReleased;
}

bool Input::IsMouseButtonUp(MouseCode mouseCode)
{
	return !s_MouseState[mouseCode].IsDown;
}

bool Input::IsMouseButtonDoubleClicked(MouseCode mouseCode)
{
	using namespace std::chrono_literals;

	if (IsMouseButtonReleased(mouseCode))
	{
		static auto before = Time::Now();
		auto now = Time::Now();
		auto difference = now - before;
		before = now;

		if (difference > 10ms && difference < 200ms)
			return true;
	}

	return false;
}

void Input::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	s_MousePosition = { (float)xpos, (float)ypos };
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	LOG_DEBUG("MouseButtonCallback: button={}, action={}, mods={:#b}", button, action, mods);
	if (action == GLFW_PRESS)
	{
		s_MouseState[button].IsDown = true;
		s_MouseState[button].IsPressed = true;
	}
	if (action == GLFW_RELEASE)
	{
		s_MouseState[button].IsDown = false;
		s_MouseState[button].IsReleased = true;
	}
}

void Input::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	s_MouseScroll = { (float)xoffset, (float)yoffset };
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	LOG_DEBUG("KeyCallback: key={}, scancode={}, action={}, mods={:#b}", key, scancode, action, mods);
	if (key == GLFW_KEY_UNKNOWN)
	{
		LOG_WARN("KeyCallback: unknown key - ignoring.");
		return;
	}
	if (action == GLFW_PRESS)
	{
		s_KeyState[key].IsDown = true;
		s_KeyState[key].IsPressed = true;
	}
	if (action == GLFW_RELEASE)
	{
		s_KeyState[key].IsDown = false;
		s_KeyState[key].IsReleased = true;
	}

	if (s_IsTextMode)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			s_KeyQueue.push(key);
	}
}

void Input::CharCallback(GLFWwindow* window, unsigned int codepoint)
{
	if (s_IsTextMode)
	{
		s_CharQueue.push(codepoint);
	}
}

void Input::BeginTextMode()
{
	s_IsTextMode = true;
}

void Input::EndTextMode()
{
	s_IsTextMode = false;
	s_KeyQueue = {};
	s_CharQueue = {};
}

bool Input::IsKeyPressed(KeyCode keyCode, bool overrideTextMode)
{
	if (s_IsTextMode && !overrideTextMode)
		return false;

	return s_KeyState[keyCode].IsPressed;
}

bool Input::IsKeyDown(KeyCode keyCode, bool overrideTextMode)
{
	if (s_IsTextMode && !overrideTextMode)
		return false;

	return s_KeyState[keyCode].IsDown;
}

bool Input::IsKeyReleased(KeyCode keyCode, bool overrideTextMode)
{
	if (s_IsTextMode && !overrideTextMode)
		return false;

	return s_KeyState[keyCode].IsReleased;
}

bool Input::IsKeyUp(KeyCode keyCode, bool overrideTextMode)
{
	if (s_IsTextMode && !overrideTextMode)
		return true;

	return !s_KeyState[keyCode].IsDown;
}

KeyCode Input::GetKey()
{
	if (!s_KeyQueue.empty())
	{
		KeyCode key = s_KeyQueue.front();
		s_KeyQueue.pop();
		return key;
	}

	return Key::Null;
}

uint32_t Input::GetChar()
{
	if (!s_CharQueue.empty())
	{
		uint32_t character = s_CharQueue.front();
		s_CharQueue.pop();
		return character;
	}

	return 0;
}

void Input::ResetStates()
{
	for (auto& state : s_MouseState)
	{
		state.IsPressed = false;
		state.IsReleased = false;
	}

	for (auto& state : s_KeyState)
	{
		state.IsPressed = false;
		state.IsReleased = false;
	}

	s_LastMousePos = s_MousePosition;
	s_MouseScroll = { 0.0f, 0.0f };

	s_KeyQueue = {};
	s_CharQueue = {};
}

