#pragma once

#include "raylib.h"
#include "raymath.h"
#include "entt.hpp"


class ScriptableEntity;


namespace Components
{
	struct Transform
	{
		Vector2 Translation = { 0.0f, 0.0f };
		float Rotation = 0.0f;
		int Index = 0;
		std::vector<int> IndexHierarchy;

		Transform() = default;
		Transform(const Transform&) noexcept = default;

		Transform(const Vector2& translation, float rotation = 0.0f, int index = 0)
			: Translation(translation), Rotation(rotation), Index(index) {}
		~Transform() { LOG_DEBUG("Transform component destroyed"); }
		operator Matrix () { return GetTransformMatrix(); }

		void Update(const std::optional<std::reference_wrapper<Transform>> parent)
		{
			// TODO: update translation and rotation
			IndexHierarchy.clear();
			if (parent)
			{
				//GlobalTranslation = Vector2Add(LocalTranslation, parent->get().GlobalTranslation);
				IndexHierarchy = parent->get().IndexHierarchy;
			}
			IndexHierarchy.emplace_back(Index);
		}

		Matrix GetTransformMatrix() const
		{
			return MatrixMultiply(GetRotationMatrix(), GetTranslationMatrix());
		}

		Matrix GetTranslationMatrix() const
		{
			return MatrixTranslate(Translation.x, Translation.y, 0.0f);
		}

		Matrix GetRotationMatrix() const
		{
			return MatrixRotateZ(Rotation);
		}
	};

	struct Hierarchy
	{
		entt::entity Parent = entt::null;
		std::vector<entt::entity> Children = {};
	};

	struct Sprite
	{
		Texture2D Texture = { 0 };
		Color Tint = WHITE;

		Sprite() = default;
		Sprite(const Components::Sprite&) = default;
		Sprite(Texture2D texture, Color tint = WHITE)
			: Texture(texture), Tint(tint) {}
		~Sprite() { LOG_DEBUG("Sprite component destroyed"); }

		operator Texture2D& () { return Texture; }
	};

	struct LineSegment
	{
		Vector2 Origin = { 0.0f, 0.0f };
		float Length = 0.0f;
		Color StrokeColor = BLACK;
		float Thickness = 1.0f;

		Vector2 GetBegin(const Transform& transform) const
		{
			return Vector2Transform(Origin, transform.GetTransformMatrix());
		}
		Vector2 GetEnd(const Transform& transform) const
		{
			Vector2 endRelative = { Origin.x + Length, Origin.y };
			return Vector2Transform(endRelative, transform.GetTransformMatrix());
		}
		Vector2 GetLocalEnd() const
		{
			return { Origin.x + Length, Origin.y };
		}
	};

	struct Arrowhead
	{
		Vector2 Offset = { 0.0f, 0.0f };
		float Width = 0.0f;
		float Height = 0.0f;

		Vector2 GetTip(const Transform& transform)
		{
			return Vector2Transform(Offset, transform.GetTransformMatrix());
		}
	};

	struct Text
	{
		std::string Content = "";
		float Size = 0.0f;
		float Spacing = 0.0f;
		Color FontColor = { 0 };
		Font Font = { 0 };

		Text() = default;
		Text(const Components::Text&) = default;
		Text(const std::string_view text, float size, const Color& color)
			: Content(text), Size(size), Spacing(size / 10.0f), FontColor(color), Font(GetFontDefault()) {}

		operator std::string_view() const { return Content; }
		operator const char* () const { return Content.c_str(); }
		Components::Text& operator=(const std::string_view newText) { Content = newText; }
	};

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
	struct Focusable
	{
		Vector2 Origin = { 0.0f, 0.0f };
		Vector2 Size = { 0.0f, 0.0f };
		bool IsFocused = false;

		Vector2 GetBegin(const Transform& transform) const
		{
			return Vector2Add(transform.Translation, Origin);
		}
		Vector2 GetEnd(const Transform& transform) const
		{
			return Vector2Add(GetBegin(transform), Size);
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
	};
}
