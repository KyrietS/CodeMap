#include "pch.hpp"
#include "TextEntity.hpp"
#include "Canvas/Components.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"
#include "Utils/Strings.hpp"
#include "Utils/System.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"

namespace
{
    void UpdateTextFocusArea(Components::Focusable& focus, const Components::Text& text)
    {
        focus.Size = Renderer::MeasureText(text);
        focus.Origin.y = -focus.Size.y;
    }

	struct Script : ScriptableEntity
	{
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

            UpdateTextFocusArea(focus, text);
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
	text = Components::Text{ content.data(), fontSize, 0.0f, VColor::Black};

	auto& focus = GetComponent<Components::Focusable>();
    UpdateTextFocusArea(focus, text);
	focus.IsFocused = true;

	return *this;
}
