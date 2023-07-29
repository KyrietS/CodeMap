#pragma once

#include "external/glfw/include/GLFW/glfw3.h"
#include "MouseCodes.hpp"
#include "KeyCodes.hpp"
#include <glm/vec2.hpp>
#include "Events/Event.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"


class Input
{
public:
	static glm::vec2 GetScreenMousePosition();
	static glm::vec2 GetWorldMousePosition();
	static glm::vec2 GetWorldMousePositionRelativeTo(glm::vec2 origin);
	static glm::vec2 GetMouseDelta();
	static glm::vec2 GetMouseWheelMove();

	static bool IsMouseButtonPressed(MouseCode);
	static bool IsMouseButtonDown(MouseCode);
	static bool IsMouseButtonReleased(MouseCode);
	static bool IsMouseButtonUp(MouseCode);
	static bool IsMouseButtonDoubleClicked(MouseCode);

	static void OnEvent(Event&);
	static void OnMouseMoved(Events::Input::MouseMoved&);
	static void OnMousePressed(Events::Input::MousePressed&);
	static void OnMouseReleased(Events::Input::MouseReleasedEvent&);
	static void OnMouseScrolled(Events::Input::MouseScrolled&);
	static void OnKeyPressed(Events::Input::KeyPressed&);
	static void OnKeyReleased(Events::Input::KeyReleased&);
	static void OnKeyTyped(Events::Input::KeyTyped&);

	static void BeginTextMode();
	static void EndTextMode();
	static bool IsKeyPressed(KeyCode, bool ignoreTextMode = false);
	static bool IsKeyDown(KeyCode, bool ignoreTextMode = false);
	static bool IsKeyReleased(KeyCode, bool ignoreTextMode = false);
	static bool IsKeyUp(KeyCode, bool ignoreTextMode = false);
	static KeyCode GetKey();
	static uint32_t GetChar();

private:
	friend class App;
	static void ResetStates();

private:
	struct InputState
	{
		bool IsDown = false;
		bool IsPressed = false;
		bool IsReleased = false;
	};

	static std::array<InputState, Mouse::LastCode + 1> s_MouseState;
	static std::array<InputState, Key::LastCode + 1> s_KeyState;

	static glm::vec2 s_MousePosition;
	static glm::vec2 s_LastMousePos;
	static glm::vec2 s_MouseScroll;

	static bool s_IsTextMode;
	static std::queue<KeyCode> s_KeyQueue;
	static std::queue<uint32_t> s_CharQueue;
};

