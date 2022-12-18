#include "pch.hpp"
#include "Input.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Canvas/Canvas.hpp"
#include "Time.hpp"


std::array<Input::InputState, GLFW_MOUSE_BUTTON_LAST+1> Input::s_MouseState;
std::array<Input::InputState, GLFW_KEY_LAST + 1> Input::s_KeyState;

glm::vec2 Input::s_MousePosition;
glm::vec2 Input::s_LastMousePos;
glm::vec2 Input::s_MouseScroll;

bool Input::s_IsTextMode = false;
std::queue<KeyCode> Input::s_KeyQueue;
std::queue<uint32_t> Input::s_CharQueue;


glm::vec2 Input::GetScreenMousePosition()
{
	return s_MousePosition;
}

glm::vec2 Input::GetWorldMousePosition()
{
	return Canvas::Camera().GetScreenToWorld(s_MousePosition);
}

glm::vec2 Input::GetWorldMousePositionRelativeTo(glm::vec2 origin)
{
	return GetWorldMousePosition() - origin;
}

glm::vec2 Input::GetMouseDelta()
{
	return s_MousePosition - s_LastMousePos;
}

glm::vec2 Input::GetMouseWheelMove()
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

void Input::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseMovedEvent>(Input::OnMouseMoved);
	dispatcher.Dispatch<MousePressedEvent>(Input::OnMousePressed);
	dispatcher.Dispatch<MouseReleasedEvent>(Input::OnMouseReleased);
	dispatcher.Dispatch<MouseScrolledEvent>(Input::OnMouseScrolled);
	dispatcher.Dispatch<KeyPressedEvent>(Input::OnKeyPressed);
	dispatcher.Dispatch<KeyReleasedEvent>(Input::OnKeyReleased);
	dispatcher.Dispatch<KeyTypedEvent>(Input::OnKeyTyped);
}

void Input::OnMouseMoved(MouseMovedEvent& event)
{
	s_MousePosition = { event.GetX(), event.GetY() };
}

void Input::OnMousePressed(MousePressedEvent& event)
{
	s_MouseState[event.GetButton()].IsDown = true;
	s_MouseState[event.GetButton()].IsPressed = true;
}

void Input::OnMouseReleased(MouseReleasedEvent& event)
{
	s_MouseState[event.GetButton()].IsDown = false;
	s_MouseState[event.GetButton()].IsReleased = true;
}

void Input::OnMouseScrolled(MouseScrolledEvent& event)
{
	s_MouseScroll = { event.GetXOffset(), event.GetYOffset() };
}

void Input::OnKeyPressed(KeyPressedEvent& event)
{
	KeyCode key = event.GetKey();

	if (not event.IsRepeated())
	{
		s_KeyState[key].IsDown = true;
		s_KeyState[key].IsPressed = true;
	}
	
	if (s_IsTextMode)
	{
		s_KeyQueue.push(key);
	}
}

void Input::OnKeyReleased(KeyReleasedEvent& event)
{
	KeyCode key = event.GetKey();
	s_KeyState[key].IsDown = false;
	s_KeyState[key].IsReleased = true;
}

void Input::OnKeyTyped(KeyTypedEvent& event)
{
	if (s_IsTextMode)
	{
		s_CharQueue.push(event.GetCodePoint());
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

