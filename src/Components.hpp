#pragma once

#include "raylib.h"
#include "raymath.h"
#include "ScriptableEntity.hpp"
#include "Utils/Print.hpp"


namespace Components
{
	struct Transform
	{
		Vector2 Translation = { 0.0f, 0.0f };
		float Rotation = 0.0f;
		int Index = 0;

		Transform() = default;
		Transform(const Components::Transform&) = default;
		Transform(const Vector2& translation, float rotation = 0.0f, int index = 0)
			: Translation(translation), Rotation(rotation), Index(index) {}
		~Transform() { print("Transform component destroyed"); }

		operator Vector2() { return GetTransform(); }

		Vector2 GetTransform() const
		{
			return Vector2Rotate(Translation, Rotation);
		}
	};

	struct Sprite
	{
		Texture2D Texture = { 0 };
		Color Tint = WHITE;

		Sprite() = default;
		Sprite(const Components::Sprite&) = default;
		Sprite(Texture2D texture, Color tint = WHITE)
			: Texture(texture), Tint(tint) {}
		~Sprite() { print("Sprite component destroyed"); }

		operator Texture2D& () { return Texture; }
	};

	struct Text
	{
		std::string Content;
		float Size;
		float Spacing;
		Color FontColor;
		Font Font;

		Text(const Components::Text&) = default;
		Text(const std::string_view text, float size, const Color& color)
			: Content(text), Size(size), Spacing(size / 10.0f), FontColor(color), Font(GetFontDefault()) {}

		operator std::string_view() const { return Content; }
		operator const char* () const { return Content.c_str(); }
		Components::Text& operator=(const std::string_view newText) { Content = newText; }
	};

	struct NativeScript
	{
		std::unique_ptr<ScriptableEntity> Instance;

		NativeScript() = default;
		NativeScript(std::unique_ptr<ScriptableEntity> ptr) : Instance{ std::move(ptr) } {}
	};

	// TODO: FocusArea for rotated rects is not supported!
	struct Focusable
	{
		bool IsFocused = false;
		Rectangle FocusArea = { 0 };
	};
}
