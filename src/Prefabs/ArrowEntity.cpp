#include "pch.hpp"
#include "ArrowEntity.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/ScriptableEntity.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Input.hpp"
#include "Render/VColor.hpp"
#include "Events/CanvasEvents.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>


namespace
{
	struct Script : ScriptableEntity
	{
		Script(EventQueue& eventQueue)
			: m_EventQueue(eventQueue)
		{
		}

		void OnCreate() override
		{
			UpdateFocusArea();

			if (Input::IsMouseButtonDown(Mouse::ButtonRight))
				m_EditMode = EditMode::End;
			else
				m_EditMode = EditMode::None;

			// TODO: Send event to focus this entity (and unfocus all others)
			GetComponent<Components::Focusable>().IsFocused = true;
		}

		void OnUpdate() override
		{
			const auto& isFocused = GetComponent<Components::Focusable>().IsFocused;
			auto& isDraggable = GetComponent<Components::Focusable>().IsDraggable;

			// Drag end edit point
			if (Input::IsMouseButtonPressed(Mouse::ButtonLeft) && IsMouseOverEndPoint())
			{
				isDraggable = false;
				m_EditMode = EditMode::End;
			}

			// Drag begin edit point
			if (Input::IsMouseButtonPressed(Mouse::ButtonLeft) && IsMouseOverBeginPoint())
			{
				isDraggable = false;
				m_EditMode = EditMode::Begin;
			}

			// Drag bezier control point
			if (Input::IsMouseButtonPressed(Mouse::ButtonLeft) && IsMouseOverControlPoint())
			{
				isDraggable = false;
				m_EditMode = EditMode::Bezier;
			}

			// Fnish all drag edits
			if (Input::IsMouseButtonReleased(Mouse::ButtonRight) || Input::IsMouseButtonReleased(Mouse::ButtonLeft))
			{
				if (m_EditMode != EditMode::None)
					m_EventQueue.Push(Events::Canvas::MakeSnapshot{});

				isDraggable = true;
				m_EditMode = EditMode::None;
			}

			// Update edit points
			if (isFocused)
			{
				switch (m_EditMode)
				{
				case EditMode::End: SetEndAt(Input::GetWorldMousePosition()); break;
				case EditMode::Begin: SetBeginAt(Input::GetWorldMousePosition()); break;
				case EditMode::Bezier: SetControlPointAt(Input::GetWorldMousePosition()); break;
				default: break;
				}
			}

			UpdateFocusArea();
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

		void SetControlPointAt(glm::vec2 pos)
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<Components::Arrow>();

			arrow.ControlPoint = pos - transform.Translation;
			UpdateFocusArea();
		}

		void UpdateFocusArea()
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<Components::Arrow>();
			auto& focus = GetComponent<Components::Focusable>();

			glm::vec2 begin = arrow.GetBegin(transform);
			glm::vec2 end = arrow.GetEnd(transform);
			glm::vec2 control = arrow.GetControlPoint(transform);

			float minX = std::min({ begin.x, end.x, control.x });
			float minY = std::min({ begin.y, end.y, control.y });
			float maxX = std::max({ begin.x, end.x, control.x });
			float maxY = std::max({ begin.y, end.y, control.y });
			float width = std::fabs(maxX - minX);
			float height = std::fabs(maxY - minY);

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

		bool IsMouseOverControlPoint()
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& arrow = GetComponent<const Components::Arrow>();
			return IsMouseOverEditPoint(arrow.GetControlPoint(transform));
		}

		bool IsMouseOverEditPoint(glm::vec2 editPoint)
		{
			float radius = ArrowEntity::EDIT_POINT_RADIUS / Canvas::Camera().GetZoom();
			auto distanceFromTheCenter = glm::length(Input::GetWorldMousePosition() - editPoint);
			return distanceFromTheCenter <= radius;
		}

		enum class EditMode
		{
			None, End, Begin, Bezier
		};

		EditMode m_EditMode = EditMode::None;
		EventQueue& m_EventQueue;
	};
}



ArrowEntity::ArrowEntity(const Entity& entity, EventQueue& eventQueue)
	: Entity(entity)
{
	AddComponent<Components::Arrow>();
	AddComponent<Components::Focusable>();

	AttachScript<::Script>(std::ref(eventQueue));
	AttachScript<MoveByDragScript>(std::ref(eventQueue));
}


ArrowEntity& ArrowEntity::Build()
{
	auto& arrow = GetComponent<Components::Arrow>();
	arrow.End = { 10.0f, 0.0f };
	arrow.StrokeColor = VColor::Blue;
	arrow.Thickness = 5.0f;

	return *this;
}
