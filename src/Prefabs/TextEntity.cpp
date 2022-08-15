#include "pch.hpp"
#include "TextEntity.hpp"
#include "Canvas/Components.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"


TextEntity::TextEntity(const Entity& entity)
	: Entity(entity)
{
	AddComponent<Components::Text>();
	AddComponent<Components::Focusable>();
	AttachScript<MoveByDragScript>();
	AttachScript<CommonCanvasEntityScript>();
}

TextEntity& TextEntity::Build(const std::string_view content, float fontSize)
{
	auto& text = GetComponent<Components::Text>();
	text = Components::Text(content, fontSize, BLACK);

	auto& focus = GetComponent<Components::Focusable>();
	focus.Size = MeasureTextEx(text.Font, text.Content.c_str(), text.Size, text.Spacing);
	focus.IsFocused = true;

	return *this;
}
