#pragma once

#include "raylib.h"
#include "raymath.h"
//#include "ScriptableEntity.hpp"
#include "Utils/Print.hpp"


class ScriptableEntity;


namespace Components
{
	// TODO: Use Matrix with helper functions instead of Translation + Rotation
	// TODO: Add helper function for getting Matrix
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

	struct Arrow
	{
		Vector2 Origin = { 0.0f, 0.0f };
		Vector2 End = { 0.0f, 0.0f };
		Color FillColor = BLACK;
		float Thickness = 1.0f;

		enum Arrowhead
		{
			None, Sharp
		};

		Arrowhead BeginHead = Arrowhead::None;
		Arrowhead EndHead = Arrowhead::None;
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

	/*
	struct NativeScript
	{
		using ScriptInstanceType = std::unique_ptr<ScriptableEntity>;

		std::vector<ScriptInstanceType> vec;

		ScriptInstanceType Instance;
		bool Active = true;

		std::function<ScriptInstanceType()> Instantiate;
		std::function<void()> DestroyScript;

		NativeScript() = default;
		NativeScript(NativeScript&& other) noexcept = default;
		NativeScript& operator=(NativeScript&&) noexcept = default;
		~NativeScript();
		//NativeScript(std::function<ScriptInstanceType()> instantiate) : Instantiate(std::move(instantiate)) {}
		//NativeScript(ScriptInstanceType ptr) : Instance{ std::move(ptr) } {}

		// Passing arguments to script's constructors is an experimental feature.
		// It should be avoided in general. If script needs some data, then it should
		// use Canvas API to get it fot itself.
		template<typename T, typename ... Args>
		void Bind(Args&&... args)
		{
			// C++20 feature
			Instantiate = [...args = std::forward<Args>(args)]() mutable { return std::make_unique<T>(std::move(args)...);  };
			DestroyScript = [this]() { Instance.reset(); };
			//vec.push_back(std::make_unique<T>(std::move(args)...));
		}
	};*/

	struct NativeScript
	{
		NativeScript() = default;
		NativeScript(NativeScript&& other) noexcept = default;
		NativeScript& operator=(NativeScript&&) noexcept = default;


		struct Instance
		{
			using ScriptInstanceType = std::unique_ptr<ScriptableEntity>;

			std::function<ScriptInstanceType()> Instantiate;

			Instance() = default;
			Instance(Instance&& other) noexcept = default;
			Instance& operator=(Instance&&) noexcept = default;
			~Instance(); // must be defined in different compilation unit

			operator bool() { return m_Instance != nullptr; }
			ScriptableEntity& operator*() { return *m_Instance; }
			ScriptableEntity* operator->() { return &(* m_Instance); }

		private:
			ScriptInstanceType m_Instance;

			friend class ScriptEngine;
		};

		std::vector<Instance> Instances;

		template<typename T, typename ... Args>
		void Bind(Args&&... args)
		{
			// C++20 feature
			auto Instantiate = [...args = std::forward<Args>(args)]() mutable { return std::make_unique<T>(std::move(args)...);  };
			Instance instance;
			instance.Instantiate = std::move(Instantiate);

			Instances.push_back(std::move(instance));
		}

	};

	// TODO: FocusArea for rotated rects is not supported!
	// TODO: Instead of Rectangle add: Vector2 origin (relative to Transform) + float width + float height
	struct Focusable
	{
		Vector2 Origin = { 0.0f, 0.0f };
		Vector2 Size = { 0.0f, 0.0f };
		bool IsFocused = false;

		Vector2 GetBegin(const Transform& transform) const
		{
			return Vector2Rotate(Vector2Add(transform.Translation, Origin), transform.Rotation);
		}
		Vector2 GetEnd(const Transform& transform) const
		{
			Vector2 begin = Vector2Add(transform.Translation, Origin);
			return Vector2Rotate(Vector2Add(begin, Size), transform.Rotation);
		}

		// WARNING: Rotation is not taken into account!!!
		Rectangle AsRectangle(const Transform& transform) const
		{
			Vector2 begin = GetBegin(transform);
			Vector2 end = GetEnd(transform);

			float minX = std::min(begin.x, end.x);
			float minY = std::min(begin.y, end.y);

			return { minX, minY, std::fabs(end.x - begin.x), std::fabs(end.y - begin.y) };
		}

		//Rectangle FocusArea = { 0 };
	};
}
