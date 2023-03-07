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
			auto isFocused = GetComponent<Components::Focusable>().IsFocused;

			if (Input::IsMouseButtonReleased(Mouse::ButtonRight))
			{
				m_EditMode = false;
			}

			if (Input::IsMouseButtonDown(Mouse::ButtonRight) && m_EditMode)
			{
				auto& transform = GetComponent<Components::Transform>();
				auto& arrow = GetComponent<Components::Arrow>();

				arrow.End = Input::GetWorldMousePosition() - transform.Translation;
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

		bool m_EditMode = true;
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
