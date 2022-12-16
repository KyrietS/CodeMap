#include "pch.hpp"
#include "LineEntity.hpp"
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
			auto isFocused = GetComponent<Components::Focusable>().IsFocused;

			if (Input::IsMouseButtonReleased(Mouse::ButtonRight))
			{
				m_EditMode = false;
			}

			if (Input::IsMouseButtonDown(Mouse::ButtonRight) && m_EditMode)
			{
				auto& transform = GetComponent<Components::Transform>();
				auto& line = GetComponent<Components::LineSegment>();

				glm::vec2 lineVector = Input::GetWorldMousePosition() - transform.Translation;
				line.Length = glm::length(lineVector);
				transform.Rotation = glm::orientedAngle({ 1.0f, 0.0f }, glm::normalize(lineVector));

				GetComponent<Components::Arrowhead>().Offset = line.GetLocalEnd();

				UpdateFocusArea(); 
			}
			if (isFocused && Input::IsKeyPressed(Key::Delete))
			{
				m_Entity.Destroy();
			}
		}

		void UpdateFocusArea()
		{
			auto& transform = GetComponent<Components::Transform>();
			auto& line = GetComponent<const Components::LineSegment>();
			auto& focus = GetComponent<Components::Focusable>();

			glm::vec2 begin = line.GetBegin(transform);
			glm::vec2 end = line.GetEnd(transform);

			float minX = std::min(begin.x, end.x);
			float minY = std::min(begin.y, end.y);
			float width = std::fabs(begin.x - end.x);
			float height = std::fabs(begin.y - end.y);

			focus.Origin = glm::vec2{ minX, minY } - glm::vec2{ begin.x, begin.y };
			focus.Size = {width, height};
		}

		bool m_EditMode = true;
	};
}



LineEntity::LineEntity(const Entity& entity)
	: Entity(entity) 
{
	AddComponent<Components::LineSegment>();
	AddComponent<Components::Arrowhead>();
	AddComponent<Components::Focusable>();

	AttachScript<::Script>();
	AttachScript<MoveByDragScript>();
	AttachScript<CommonCanvasEntityScript>();
}


LineEntity& LineEntity::Build(Vector2 end)
{
	auto& line = GetComponent<Components::LineSegment>();
	line.Origin = {0.0f, 0.0f};
	line.Length = 0.0f;
	line.StrokeColor = VColor::Blue;
	line.Thickness = 5.0f;

	auto& arrowhead = GetComponent<Components::Arrowhead>();
	arrowhead.Offset = { 0.0f, 0.0f };
	arrowhead.Height = 10.0f;
	arrowhead.Width = 30.0f;

	return *this;
}
