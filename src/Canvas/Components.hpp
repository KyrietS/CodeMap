#pragma once

#include "entt/entt.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include "Canvas/Box.hpp"
#include "Render/VColor.hpp"
#include "Render/BlendMode.hpp"


class ScriptableEntity;
class ScriptEngine;

namespace Components
{
	struct Transform
	{
		glm::vec2 Translation = { 0.0f, 0.0f };
		float Rotation = 0.0f; // In radians
		int Index = 0;
		std::vector<int> IndexHierarchy;

		// Global parent translation
		glm::vec2 ParentTranslation = { 0.0f, 0.0f };

		glm::vec2 GetGlobalTranslation() const
		{
			return ParentTranslation + Translation;
		}

		glm::mat4 GetTranslationMatrix2() const
		{
			return glm::translate(glm::vec3{ Translation.x, Translation.y, 0.0f });
		}

		glm::mat4 GetGlobalTranslationMatrix2() const
		{
			const auto globalTranslation = GetGlobalTranslation();
			return glm::translate(glm::vec3{ globalTranslation.x, globalTranslation.y, 0.0f });
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
		Transform(Transform&&) noexcept = default;
		Transform& operator=(const Transform&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;
		

		Transform(const glm::vec2& translation, float rotation = 0.0f, int index = 0)
			: Translation(translation), Rotation(rotation), Index(index) {}
		~Transform() { LOG_DEBUG("Transform component destroyed"); }

		void Update(const std::optional<std::reference_wrapper<Transform>> parent)
		{
			// TODO: update rotation
			IndexHierarchy.clear();
			if (parent)
			{
				ParentTranslation = parent->get().GetGlobalTranslation();
				IndexHierarchy = parent->get().IndexHierarchy;
			}
			else
			{
				ParentTranslation = { 0.0f, 0.0f };
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
		std::shared_ptr<unsigned int> TextureId = nullptr;
		int Width = 0;
		int Height = 0;
	};

	struct Arrow
	{
		glm::vec2 End = { 1.0f, 0.0f };
		glm::vec4 StrokeColor = VColor::Black;
		glm::vec4 ArrowheadColor = VColor::Black;
		float Thickness = 1.0f;
		std::optional<glm::vec2> ControlPoint = std::nullopt;

		glm::vec2 GetBegin(const Transform& transform) const
		{
			return transform.Translation;
		}

		glm::vec2 GetEnd(const Transform& transform) const
		{
			return transform.GetTransform() * glm::vec4{ End.x, End.y, 0.0f, 1.0f };
		}

		glm::vec2 GetControlPoint(const Transform& transform) const
		{
			glm::vec2 control = ControlPoint.value_or(glm::mix(glm::vec2{ 0.0f, 0.0f }, End, 0.5f));
			return transform.GetTransform() * glm::vec4{ control.x, control.y, 0.0f, 1.0f };
		}

		float GetLength() const
		{
			return glm::length(End);
		}

		float GetEndAngle() const
		{
			glm::vec2 control = ControlPoint.value_or(glm::vec2{ 0.0f, 0.0f });
			return glm::orientedAngle(glm::vec2{ 1.0f, 0.0f }, glm::normalize(End - control));
		}
	};

	struct Highlight
	{
		glm::vec4 Color = VColor::Yellow;
		Render::BlendMode BlendMode = Render::BlendMode::Multiply;
		
		std::vector<glm::vec2> Points = {};
		std::optional<size_t> SelectedPointIndex = std::nullopt;

		std::vector<glm::vec2> GetWorldPoints(const Transform& transform) const
		{
			std::vector<glm::vec2> points;
			points.reserve(Points.size());
			for (const auto& point : Points)
			{
				points.emplace_back(transform.GetTransform() * glm::vec4{ point.x, point.y, 0.0f, 1.0f });
			}
			return points;
		}

		void AddPoint(const Transform& transform, const glm::vec2& point)
		{
			Points.emplace_back(glm::inverse(transform.GetTransform()) * glm::vec4{ point.x, point.y, 0.0f, 1.0f });
		}
	};

	struct Text
	{
		std::string Content = ""; // In UTF-8
		float FontSize = 0.0f;
		float LetterSpacing = 0.0f;
		glm::vec4 FontColor = {};

		bool IsCursorShown = false;
		unsigned int FontId = 0;
	};

	struct NativeScript
	{
		NativeScript() = default;
		NativeScript(const NativeScript&) noexcept = default;
		NativeScript(NativeScript&& other) noexcept = default;
		NativeScript& operator=(NativeScript&&) noexcept = default;


		struct Instance
		{
			using ScriptInstanceType = std::unique_ptr<ScriptableEntity>;

			std::function<ScriptInstanceType()> Instantiate;

			Instance() = default;
			Instance(Instance&& other) noexcept = default;
			Instance& operator=(Instance&&) noexcept = default;
			Instance(const Instance& other) noexcept;
			~Instance(); // must be defined in different compilation unit

			operator bool() { return m_Instance != nullptr; }
			ScriptableEntity& operator*() { return *m_Instance; }
			ScriptableEntity* operator->() { return &(* m_Instance); }

		private:
			ScriptInstanceType m_Instance;

			friend class ::ScriptEngine;
		};

		std::vector<Instance> Instances;

		template<typename T, typename ... Args>
		void Bind(Args&&... args)
		{
			// C++20 feature
			auto Instantiate = [...args = std::forward<Args>(args)]() { return std::make_unique<T>(args...);  };
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
		bool IsDraggable = true;
		glm::vec4 BorderColor = DefaultBorderColor;

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

		void ResetBorderColor()
		{
			BorderColor = DefaultBorderColor;
		}

		static constexpr glm::vec4 DefaultBorderColor = VColor::Blue;
	};
}
