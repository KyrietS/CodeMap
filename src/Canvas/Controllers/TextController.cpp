#include "pch.hpp"
#include "TextController.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"
#include "Prefabs/TextEntity.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"
#include <clip.h>


void TextController::OnUpdate()
{
	auto texts = Canvas::GetAllEntitiesWith<Components::Focusable, Components::Text>();

	bool noTextIsFocued = std::all_of(texts.begin(), texts.end(), [](Entity entity) {
		return !entity.GetComponent<Components::Focusable>().IsFocused;
	});

	if (Input::IsMouseButtonDoubleClicked(Mouse::ButtonLeft) && noTextIsFocued)
	{
		AddText("text");
	}
}

void TextController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Canvas::Paste>(BIND_EVENT(TextController::OnPasteEvent));
}

void TextController::OnPasteEvent(const Events::Canvas::Paste&)
{
	if (clip::has(clip::text_format()))
	{
		std::string clipboardText;
		if (clip::get_text(clipboardText))
		{
			if (auto focusedText = GetFocusedText())
			{
				focusedText->GetComponent<Components::Text>().Content += clipboardText;
				m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
			}
			else
			{
				AddText(clipboardText);
			}
		}
	}
}

std::optional<Entity> TextController::GetFocusedText()
{
	auto texts = Canvas::GetAllEntitiesWith<Components::Focusable, Components::Text>();
	auto it = std::find_if(texts.begin(), texts.end(), [](Entity entity) {
		return entity.GetComponent<Components::Focusable>().IsFocused;
	});

	return it != texts.end() ? std::optional<Entity>(*it) : std::nullopt;
}

void TextController::AddText(const std::string& text)
{
	const int FONT_SIZE = 32;
	auto entity = TextEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition()), m_EventQueue).Build(text, FONT_SIZE);
	m_EventQueue.Push(Events::Canvas::SetFocus{ entity });
	m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
}
