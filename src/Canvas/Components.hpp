#pragma once

#include "entt.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include "Canvas/Box.hpp"
#include "Render/VColor.hpp"


class ScriptableEntity;


namespace Components
{
	struct Transform
	{
		glm::vec2 Translation = { 0.0f, 0.0f };
		float Rotation = 0.0f; // In radians
		int Index = 0;
		std::vector<int> IndexHierarchy;

		// This includes parent's translation
		glm::vec2 GlobalTranslation = { 0.0f, 0.0f };

		glm::mat4 GetTranslationMatrix2() const
		{
			return glm::translate(glm::vec3{ Translation.x, Translation.y, 0.0f });
		}

		glm::mat4 GetGlobalTranslationMatrix2() const
		{
			return glm::translate(glm::vec3{ GlobalTranslation.x, GlobalTranslation.y, 0.0f });
		}

		glm::mat4 GetRotationMatrix2() const
		{
			// Rotation along Z-axis
			return glm::rotate(Rotation, glm::vec3{ 0.0f, 0.0f, 1.0f });
		}

		glm::mat4 GetLocalTransform() const
		{
			return GetTranslationMatrix2() * GetRotationMatrix2();
		}

		glm::mat4 GetTransform() const
		{
			return GetGlobalTranslationMatrix2() * GetRotationMatrix2();
		}

		Transform() = default;
		Transform(const Transform&) noexcept = default;

		Transform(const glm::vec2& translation, float rotation = 0.0f, int index = 0)
			: Translation(translation), Rotation(rotation), Index(index) {}
		~Transform() { LOG_DEBUG("Transform component destroyed"); }

		void Update(const std::optional<std::reference_wrapper<Transform>> parent)
		{
			// TODO: update rotation
			IndexHierarchy.clear();
			GlobalTranslation = Translation;
			if (parent)
			{
				GlobalTranslation = Translation + parent->get().GlobalTranslation;
				IndexHierarchy = parent->get().IndexHierarchy;
			}
			IndexHierarchy.emplace_back(Index);
		}
	};

	struct Hierarchy
	{
		entt::entity Parent = entt::null;
		std::vector<entt::entity> Children = {};
	};

	struct Image
	{
		unsigned int TextureId = 0;
		int Width = 0;
		int Height = 0;
	};

	struct LineSegment
	{
		glm::vec2 Origin = { 0.0f, 0.0f };
		float Length = 0.0f;
		glm::vec4 StrokeColor = VColor::Black;
		float Thickness = 1.0f;

		glm::vec2 GetBegin(const Transform& transform) const
		{
			return transform.GetTransform() * glm::vec4{ Origin.x, Origin.y, 0.0f, 1.0f };
		}

		glm::vec2 GetEnd(const Transform& transform) const
		{
			glm::vec4 endRelative = { Origin.x + Length, Origin.y, 0.0f, 1.0f };
			return transform.GetTransform() * endRelative;
		}

		glm::vec2 GetLocalEnd() const
		{
			return { Origin.x + Length, Origin.y };
		}
	};

	struct Arrowhead
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Width = 0.0f;
		float Height = 0.0f;

		glm::vec2 GetTip(const Transform& transform)
		{
			return transform.GetTransform() * glm::vec4{ Offset.x, Offset.y, 0.0f, 1.0f };
		}
	};

	struct Text
	{
		std::string Content = "";
		float FontSize = 0.0f;
		float LetterSpacing = 0.0f;
		glm::vec4 FontColor = {};

		// TODO: Font
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
		glm::vec2 Origin = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.0f, 0.0f };

		bool IsFocused = false;

		glm::vec2 GetBegin(const Transform& transform) const
		{
			return transform.Translation + Origin;
		}

		glm::vec2 GetEnd(const Transform& transform) const
		{
			return GetBegin(transform) + Size;
		}

		Box AsBox(const Transform& transform) const
		{
			glm::vec2 begin = GetBegin(transform);
			glm::vec2 end = GetEnd(transform);

			float minX = std::min(begin.x, end.x);
			float minY = std::min(begin.y, end.y);

			return { minX, minY, std::fabs(end.x - begin.x), std::fabs(end.y - begin.y) };
		}
	};
}
