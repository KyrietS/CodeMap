#pragma once

#include "external/glfw/include/GLFW/glfw3.h"
#include "MouseCodes.hpp"
#include "KeyCodes.hpp"
#include <glm/vec2.hpp>
#include "Events/Event.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"
#include "Time.hpp"

class CanvasCamera;

class Input
{
public:
	static glm::vec2 GetScreenMousePosition();
	// TODO: These "World" functions should be in a separate class, eg. InputProvider
	// and the camera should be stored there.
	static glm::vec2 GetWorldMousePosition(const CanvasCamera&);
	static glm::vec2 GetWorldMousePositionRelativeTo(const CanvasCamera&, glm::vec2 origin);
	static glm::vec2 GetMouseDelta();
	static glm::vec2 GetMouseWheelMove();

	static bool IsMouseButtonPressed(MouseCode);
	static bool IsMouseButtonDown(MouseCode);
	static bool IsMouseButtonReleased(MouseCode);
	static bool IsMouseButtonUp(MouseCode);
	static bool IsMouseButtonClicked(MouseCode);
	static bool IsMouseButtonDoublePressed(MouseCode);
	static bool IsMouseButtonDoubleClicked(MouseCode);
	static bool IsMouseDragged(MouseCode);


	static void OnEvent(Event&);
	static void OnMouseMoved(Events::Input::MouseMoved&);
	static void OnMousePressed(Events::Input::MousePressed&);
	static void OnMouseReleased(Events::Input::MouseReleased&);
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

	static bool IsControlDown(); // Checks if any Control key is pressed even if a window is not focused

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
	static std::array<TimePoint, Mouse::LastCode + 1> s_LastClickTime;
	static std::array<TimePoint, Mouse::LastCode + 1> s_CurrentClickTime;

	static glm::vec2 s_MousePosition;
	static glm::vec2 s_LastMousePos;
	static glm::vec2 s_MouseScroll;
	static glm::vec2 s_MouseClickPos;

	static bool s_IsTextMode;
	static std::queue<KeyCode> s_KeyQueue;
	static std::queue<uint32_t> s_CharQueue;
};

