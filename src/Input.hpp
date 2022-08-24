#pragma once

#include "raylib.h"
#include "external/glfw/include/GLFW/glfw3.h"
#include "MouseCodes.hpp"
#include "KeyCodes.hpp"


struct GLFWwindow;

class Input
{
public:
	static void Init();
	static void PollEvents();
	static void PollEventsOrWait();

	static Vector2 GetScreenMousePosition();
	static Vector2 GetWorldMousePosition();
	static Vector2 GetWorldMousePositionRelativeTo(Vector2 origin);
	static Vector2 GetMouseDelta();
	static Vector2 GetMouseWheelMove();

	static bool IsMouseButtonPressed(MouseCode);
	static bool IsMouseButtonDown(MouseCode);
	static bool IsMouseButtonReleased(MouseCode);
	static bool IsMouseButtonUp(MouseCode);
	static bool IsMouseButtonDoubleClicked(MouseCode);

	static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharCallback(GLFWwindow* window, unsigned int codepoint);

	static void BeginTextMode();
	static void EndTextMode();
	static bool IsKeyPressed(KeyCode, bool overrideTextMode = false);
	static bool IsKeyDown(KeyCode, bool overrideTextMode = false);
	static bool IsKeyReleased(KeyCode, bool overrideTextMode = false);
	static bool IsKeyUp(KeyCode, bool overrideTextMode = false);
	static KeyCode GetKey();
	static uint32_t GetChar();

private:
	static void ResetStates();

private:
	static GLFWwindow* s_GlfwWindow;
	struct InputState
	{
		bool IsDown = false;
		bool IsPressed = false;
		bool IsReleased = false;
	};

	static std::array<InputState, GLFW_MOUSE_BUTTON_LAST+1> s_MouseState;
	static std::array<InputState, GLFW_KEY_LAST + 1> s_KeyState;

	static Vector2 s_MousePosition;
	static Vector2 s_LastMousePos;
	static Vector2 s_MouseScroll;

	static bool s_IsTextMode;
	static std::queue<KeyCode> s_KeyQueue;
	static std::queue<uint32_t> s_CharQueue;
};

