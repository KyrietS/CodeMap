#include "pch.hpp"
#include "Input.hpp"
#include "Events/EventDispatcher.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Time.hpp"


std::array<Input::InputState, GLFW_MOUSE_BUTTON_LAST+1> Input::s_MouseState;
std::array<Input::InputState, GLFW_KEY_LAST + 1> Input::s_KeyState;
std::array<TimePoint, GLFW_MOUSE_BUTTON_LAST + 1> Input::s_LastClickTime;
std::array<TimePoint, GLFW_MOUSE_BUTTON_LAST + 1> Input::s_CurrentClickTime;

glm::vec2 Input::s_MousePosition;
glm::vec2 Input::s_LastMousePos;
glm::vec2 Input::s_MouseScroll;
glm::vec2 Input::s_MouseClickPos;

bool Input::s_IsTextMode = false; // TODO: Unused anymore. Remove when ready.
std::queue<KeyCode> Input::s_KeyQueue;
std::queue<uint32_t> Input::s_CharQueue;


glm::vec2 Input::GetScreenMousePosition()
{
	return s_MousePosition;
}

glm::vec2 Input::GetWorldMousePosition(const CanvasCamera& camera)
{
	return camera.GetScreenToWorld(s_MousePosition);
}

glm::vec2 Input::GetWorldMousePositionRelativeTo(const CanvasCamera& camera, glm::vec2 origin)
{
	return GetWorldMousePosition(camera) - origin;
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

bool Input::IsMouseButtonClicked(MouseCode mouseCode)
{
	// If mouse button is released in the same position it was pressed
	if (IsMouseButtonReleased(mouseCode) && s_MousePosition == s_MouseClickPos)
	{
		return true;
	}

	return false;
}

bool Input::IsMouseButtonDoublePressed(MouseCode mouseCode)
{
	using namespace std::chrono_literals;

	if (IsMouseButtonPressed(mouseCode))
	{
		auto difference = Time::Now() - s_CurrentClickTime[mouseCode];
		auto diffInMs = std::chrono::duration_cast<std::chrono::milliseconds>(difference);
		if (difference < 200ms)
			return true;
	}
	return false;
}

bool Input::IsMouseButtonDoubleClicked(MouseCode mouseCode)
{
	using namespace std::chrono_literals;

	if (IsMouseButtonReleased(mouseCode))
	{
		auto difference = Time::Now() - s_LastClickTime[mouseCode];

		if (difference < 200ms)
			return true;
	}

	return false;
}

bool Input::IsMouseDragged(MouseCode mouseCode)
{
	return IsMouseButtonDown(mouseCode) && GetMouseDelta() != glm::vec2(0.0f);
}

void Input::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Input::MouseMoved>(Input::OnMouseMoved);
	dispatcher.Dispatch<Events::Input::MousePressed>(Input::OnMousePressed);
	dispatcher.Dispatch<Events::Input::MouseReleased>(Input::OnMouseReleased);
	dispatcher.Dispatch<Events::Input::MouseScrolled>(Input::OnMouseScrolled);
	dispatcher.Dispatch<Events::Input::KeyPressed>(Input::OnKeyPressed);
	dispatcher.Dispatch<Events::Input::KeyReleased>(Input::OnKeyReleased);
	dispatcher.Dispatch<Events::Input::KeyTyped>(Input::OnKeyTyped);
}

void Input::OnMouseMoved(Events::Input::MouseMoved& event)
{
	s_LastMousePos = s_MousePosition;
	s_MousePosition = { event.GetX(), event.GetY() };
}

void Input::OnMousePressed(Events::Input::MousePressed& event)
{
	s_MouseState[event.GetButton()].IsDown = true;
	s_MouseState[event.GetButton()].IsPressed = true;
	s_MouseClickPos = s_MousePosition;
}

void Input::OnMouseReleased(Events::Input::MouseReleased& event)
{
	s_MouseState[event.GetButton()].IsDown = false;
	s_MouseState[event.GetButton()].IsReleased = true;
	s_LastClickTime[event.GetButton()] = s_CurrentClickTime[event.GetButton()];
	s_CurrentClickTime[event.GetButton()] = Time::Now();
}

void Input::OnMouseScrolled(Events::Input::MouseScrolled& event)
{
	s_MouseScroll = { event.GetXOffset(), event.GetYOffset() };
}

void Input::OnKeyPressed(Events::Input::KeyPressed& event)
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

void Input::OnKeyReleased(Events::Input::KeyReleased& event)
{
	KeyCode key = event.GetKey();
	s_KeyState[key].IsDown = false;
	s_KeyState[key].IsReleased = true;
}

void Input::OnKeyTyped(Events::Input::KeyTyped& event)
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

bool Input::IsKeyPressed(KeyCode keyCode, bool ignoreTextMode)
{
	if (s_IsTextMode && !ignoreTextMode)
		return false;

	return s_KeyState[keyCode].IsPressed;
}

bool Input::IsKeyDown(KeyCode keyCode, bool ignoreTextMode)
{
	if (s_IsTextMode && !ignoreTextMode)
		return false;

	return s_KeyState[keyCode].IsDown;
}

bool Input::IsKeyReleased(KeyCode keyCode, bool ignoreTextMode)
{
	if (s_IsTextMode && !ignoreTextMode)
		return false;

	return s_KeyState[keyCode].IsReleased;
}

bool Input::IsKeyUp(KeyCode keyCode, bool ignoreTextMode)
{
	if (s_IsTextMode && !ignoreTextMode)
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

bool Input::IsControlDown()
{
#ifdef _WIN32
	return GetAsyncKeyState(VK_CONTROL) & 0x8000;
#else
	return IsKeyDown(Key::LeftControl) || IsKeyDown(Key::RightControl);
#endif
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

	s_MouseScroll = { 0.0f, 0.0f };

	s_KeyQueue = {};
	s_CharQueue = {};
}

