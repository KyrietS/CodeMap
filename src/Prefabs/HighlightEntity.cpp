#include "pch.hpp"
#include "HighlightEntity.hpp"
#include "Canvas/ScriptableEntity.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"

namespace
{
	struct Script : ScriptableEntity
	{
		Script(Entity entity, EventQueue& eventQueue)
			: m_Entity(entity), m_EventQueue(eventQueue)
		{
		}

		void OnUpdate() override
		{
			UpdateFocusArea();
			bool isFocused = GetComponent<Components::Focusable>().IsFocused;
			if (not isFocused)
				return;

			auto& highlight = GetComponent<Components::Highlight>();
			const auto& transform = GetComponent<Components::Transform>();
			if (Input::IsKeyDown(Key::LeftControl) && Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			{
				auto point = Input::GetWorldMousePosition();
				highlight.AddPoint(transform, point);
				highlight.SelectedPointIndex = highlight.Points.size() - 1;
				LOG_DEBUG("Added point to Highlight entity");

				UpdateFocusArea();
			}

			if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			{
				highlight.SelectedPointIndex = std::nullopt;
				for (size_t index = 0; index < highlight.Points.size(); index++)
				{
					const auto& point = highlight.Points[index];
					if (IsMouseOverEditPoint(point))
					{
						m_ControlPointIsBeingDragged = true;
						highlight.SelectedPointIndex = index;
						GetComponent<Components::Focusable>().IsDraggable = false;
						LOG_DEBUG("Selected point on Highlight entity");
					}
				}
			}

			if (Input::IsMouseButtonReleased(Mouse::ButtonLeft) && m_ControlPointIsBeingDragged)
			{
				m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
			}

			if (Input::IsMouseButtonDown(Mouse::ButtonLeft)
				&& m_ControlPointIsBeingDragged 
				&& highlight.SelectedPointIndex < highlight.Points.size())
			{
				DragSelectedPoint(highlight.SelectedPointIndex.value());
				UpdateFocusArea();
			}

			if (Input::IsMouseButtonReleased(Mouse::ButtonLeft))
			{
				GetComponent<Components::Focusable>().IsDraggable = true;
				m_ControlPointIsBeingDragged = false;
			}

			if (Input::IsKeyPressed(Key::Delete))
			{
				if (highlight.SelectedPointIndex.has_value())
				{
					highlight.Points.erase(highlight.Points.begin() + highlight.SelectedPointIndex.value());
					highlight.SelectedPointIndex = std::nullopt;
					LOG_DEBUG("Deleted point from Highlight entity");
					if (highlight.Points.size() < 2)
						m_Entity.Destroy();
				}
				else
				{
					m_Entity.Destroy();
				}
				m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
			}
		}

		void DragSelectedPoint(size_t selectedPointIndex)
		{
			auto& highlight = GetComponent<Components::Highlight>();
			const auto& transform = GetComponent<Components::Transform>();
			const auto mousePosition = Input::GetWorldMousePosition();
			const auto localMousePosition = mousePosition - transform.GetGlobalTranslation();

			highlight.Points.at(selectedPointIndex) = localMousePosition;
		}

		bool IsMouseOverEditPoint(glm::vec2 editPoint)
		{
			const auto& transform = GetComponent<Components::Transform>();
			glm::vec2 worldEditPoint = transform.GetGlobalTranslation() + editPoint;
			float radius = HighlightEntity::EDIT_POINT_RADIUS / Canvas::Camera().GetZoom();
			auto distanceFromTheCenter = glm::length(Input::GetWorldMousePosition() - worldEditPoint);
			return distanceFromTheCenter <= radius;
		}

		void UpdateFocusArea()
		{
			const auto& points = GetComponent<Components::Highlight>().Points;
			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min();
			float maxY = std::numeric_limits<float>::min();

			for (const auto& point : points)
			{
				minX = std::min(minX, point.x);
				minY = std::min(minY, point.y);
				maxX = std::max(maxX, point.x);
				maxY = std::max(maxY, point.y);
			}

			glm::vec2 origin = { minX, minY };
			glm::vec2 size = { maxX - minX, maxY - minY };
			GetComponent<Components::Focusable>().Origin = origin;
			GetComponent<Components::Focusable>().Size = size;
		}

		Entity m_Entity;
		EventQueue& m_EventQueue;
		bool m_ControlPointIsBeingDragged = false;
	};
}

HighlightEntity::HighlightEntity(const Entity& entity, EventQueue& eventQueue)
	: Entity(entity)
{
	AddComponent<Components::Highlight>();
	AddComponent<Components::Focusable>();

	AttachScript<::Script>(entity, std::ref(eventQueue));
	AttachScript<MoveByDragScript>(std::ref(eventQueue));
}
