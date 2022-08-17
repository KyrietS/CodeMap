#include "pch.hpp"
#include "TextEntity.hpp"
#include "Canvas/Components.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"

#include "raylib.h"
#include "external/glfw/include/GLFW/glfw3.h"


namespace
{


	struct Script : ScriptableEntity
	{
		// static void MyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		// {
		// 	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == KEY_BACKSPACE)
		// 	{
		// 		print("Key = {} ({})", key, key == KEY_BACKSPACE);
		// 	}
		// }

		// void OnCreate() override
		// {
		// 	GLFWwindow* glfwWindow = glfwGetCurrentContext();
		// 	glfwSetKeyCallback(glfwWindow, MyCallback);
		// 	print("Registered new KeyCallback in glfw");
		// }

		void OnUpdate() override
		{
			auto& focus = GetComponent<Components::Focusable>();
			auto& text = GetComponent<Components::Text>();

			if (focus.IsFocused)
			{
				auto& content = text.Content;
				while (int character = GetCharPressed())
				{
					print("Char code = {}", character);
					content += (char)character;
				}
				if (IsKeyPressed(KEY_ENTER)) content += '\n';

				while (int keyCode = GetKeyPressed())
				{
					print("Key code = {}", keyCode);
					if (keyCode == KEY_BACKSPACE)
					{
						content = content.substr(0, content.size() - 1);
					}
				}

				// Holding backspace doesn't work :-/
				// It's gonna be difficult to overcome with raylib
				if (IsKeyPressed(KEY_BACKSPACE))
				{
					content = content.substr(0, content.size() - 1);
				}
			}

			focus.Size = MeasureTextEx(text.Font, text, text.Size, text.Spacing);
		}


	};
}


TextEntity::TextEntity(const Entity& entity)
	: Entity(entity)
{
	AddComponent<Components::Text>();
	AddComponent<Components::Focusable>();

	AttachScript<::Script>();
	AttachScript<MoveByDragScript>();
	AttachScript<CommonCanvasEntityScript>();
}

TextEntity& TextEntity::Build(const std::string_view content, float fontSize)
{
	auto& text = GetComponent<Components::Text>();
	text = Components::Text(content, fontSize, BLACK);
	text.Spacing = 1.0f;

	auto& focus = GetComponent<Components::Focusable>();
	focus.Size = MeasureTextEx(text.Font, text.Content.c_str(), text.Size, text.Spacing);
	focus.IsFocused = true;

	return *this;
}
