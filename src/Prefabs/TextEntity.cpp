#include "pch.hpp"
#include "TextEntity.hpp"
#include "Canvas/Components.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"
#include "Utils/Strings.hpp"
#include "Utils/System.hpp"
#include "Input.hpp"

#include "raylib.h"

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
		//  GLFWwindow* glfwWindow = glfwGetCurrentContext();
		// 	glfwSetKeyCallback(glfwWindow, MyCallback);
		// 	print("Registered new KeyCallback in glfw");
		// }

		void OnUpdate() override
		{
			auto& focus = GetComponent<Components::Focusable>();
			auto& text = GetComponent<Components::Text>();

			if (focus.IsFocused)
			{
				Input::BeginTextMode();
				m_IsTextModeActive = true;

				auto& content = text.Content;
				while (char32_t character = Input::GetChar())
				{
					content += Utils::Strings::ToUtf8(character);
				}
				while (KeyCode keyCode = Input::GetKey())
				{
					LOG_DEBUG("Key code = {}", keyCode);
					if (keyCode == Key::Backspace)
						content = content.substr(0, content.size() - 1);
					if (keyCode == Key::Enter)
						content += '\n';
				}
			}
			else if (m_IsTextModeActive)
			{
				Input::EndTextMode();
				m_IsTextModeActive = false;
			}

			focus.Size = MeasureTextEx(text.Font, text, text.Size, text.Spacing);
		}

		bool m_IsTextModeActive = false;
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

	// The last polish Unicode character is 380
	std::array<int, 381> glyphs;
	for (int i = 0; i < glyphs.size(); i++)
	{
		glyphs[i] = i;
	}

	// TODO: Load one font the entire program. Loading it for every text is a huge memory waste.
	std::string fontPath = Utils::System::GetSystemFontDirPath() + "\\calibri.ttf";
	text.Font = LoadFontEx(fontPath.c_str(), 64, glyphs.data(), (int)glyphs.size());
	SetTextureFilter(text.Font.texture, TEXTURE_FILTER_BILINEAR);

	auto& focus = GetComponent<Components::Focusable>();
	focus.Size = MeasureTextEx(text.Font, text.Content.c_str(), text.Size, text.Spacing);
	focus.IsFocused = true;

	return *this;
}
