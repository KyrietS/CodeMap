#include "pch.hpp"
#include "ArrowEntity.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/ScriptableEntity.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"
#include "Input.hpp"
#include "Render/VColor.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>


namespace
{
	struct Script : ScriptableEntity
	{
		void OnUpdate() override
		{
			auto& isFocused = GetComponent<Components::Focusable>().IsFocused;

			// Drag end edit point
			if (Input::IsMouseButtonDown(Mouse::ButtonLeft) && IsMouseOverEndPoint())
			{
				isFocused = false;
				m_EditMode = EditMode::End;
			}

			// Drag begin edit point
			if (Input::IsMouseButtonDown(Mouse::ButtonLeft) && IsMouseOverBeginPoint())
			{
				isFocused = false;
				m_EditMode = EditMode::Begin;
			}

			// Fnish all drag edits
			if (Input::IsMouseButtonReleased(Mouse::ButtonRight) || Input::IsMouseButtonReleased(Mouse::ButtonLeft))
			{
				m_EditMode = EditMode::None;
			}

			switch (m_EditMode)
			{
			case EditMode::End: SetEndAt(Input::GetWorldMousePosition()); break;
			case EditMode::Begin: SetBeginAt(Input::GetWorldMousePosition()); break;
			default: break;
			}

			if (isFocused && Input::IsKeyPressed(Key::Delete))
			{
				m_Entity.Destroy();
			}
		}

		void SetEndAt(glm::vec2 pos)
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<Components::Arrow>();

			arrow.End = pos - transform.Translation;
			UpdateFocusArea();
		}

		void SetBeginAt(glm::vec2 pos)
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<Components::Arrow>();

			arrow.End -= (pos - transform.Translation);
			transform.Translation = pos;
			UpdateFocusArea();
		}

		void UpdateFocusArea()
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<const Components::Arrow>();
			auto& focus = GetComponent<Components::Focusable>();

			glm::vec2 begin = arrow.GetBegin(transform);
			glm::vec2 end = arrow.GetEnd(transform);

			float minX = std::min(begin.x, end.x);
			float minY = std::min(begin.y, end.y);
			float width = std::fabs(begin.x - end.x);
			float height = std::fabs(begin.y - end.y);

			focus.Origin = glm::vec2{ minX, minY } - glm::vec2{ begin.x, begin.y };
			focus.Size = { width, height };
		}

		bool IsMouseOverEndPoint()
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<const Components::Arrow>();
			return IsMouseOverEditPoint(arrow.GetEnd(transform));
		}

		bool IsMouseOverBeginPoint()
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<const Components::Arrow>();
			return IsMouseOverEditPoint(arrow.GetBegin(transform));
		}

		bool IsMouseOverEditPoint(glm::vec2 editPoint)
		{
			float radius = ArrowEntity::EDIT_POINT_RADIUS / Canvas::Camera().GetZoom();
			auto distanceFromTheCenter = glm::length(Input::GetWorldMousePosition() - editPoint);
			return distanceFromTheCenter <= radius;
		}

		enum class EditMode
		{
			None, End, Begin
		};

		EditMode m_EditMode = EditMode::End;
	};
}



ArrowEntity::ArrowEntity(const Entity& entity)
	: Entity(entity)
{
	AddComponent<Components::Arrow>();
	AddComponent<Components::Focusable>();

	AttachScript<::Script>();
	AttachScript<MoveByDragScript>();
	AttachScript<CommonCanvasEntityScript>();
}


ArrowEntity& ArrowEntity::Build()
{
	auto& arrow = GetComponent<Components::Arrow>();
	arrow.End = { 10.0f, 0.0f };
	arrow.StrokeColor = VColor::Blue;
	arrow.Thickness = 5.0f;

	return *this;
}
