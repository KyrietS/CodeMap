#include "pch.hpp"
#include "LineEntity.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/ScriptableEntity.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Input.hpp"


namespace
{
	struct Script : ScriptableEntity
	{
		
		void OnUpdate() override
		{
			auto isFocused = GetComponent<Components::Focusable>().IsFocused;

			if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
			{
				m_EditMode = false;
			}

			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && m_EditMode)
			{
				auto transform = GetComponent<Components::Transform>().GetTransform();
				auto& line = GetComponent<Components::Arrow>();
				Vector2 end = Vector2Subtract(Input::GetWorldMousePosition(), transform);
				line.End = end;
				UpdateFocusArea();
			}
			if (isFocused && IsKeyPressed(KEY_DELETE))
			{
				m_Entity.Destroy();
			}
		}

		void UpdateFocusArea()
		{
			//auto transform = GetComponent<Components::Transform>().GetTransform();
			auto& line = GetComponent<const Components::Arrow>();
			auto& focus = GetComponent<Components::Focusable>();

			focus.Origin = line.Origin;
			focus.Size = line.End;

			//Vector2 begin = Vector2Add(transform, line.Origin);
			//Vector2 end = Vector2Add(transform, line.End);

			//float minX = std::min(begin.x, end.x);
			//float minY = std::min(begin.y, end.y);
			//float width = std::fabs(begin.x - end.x);
			//float height = std::fabs(begin.y - end.y);

			//focus.FocusArea = { minX, minY, width, height };
		}

		bool m_EditMode = true;
	};
}



LineEntity::LineEntity(const Entity& entity)
	: Entity(entity) 
{
	AddComponent<Components::Arrow>();
	AddComponent<Components::Focusable>();
	// TODO: Add ability to attach multiple scripts
	AddComponent<Components::NativeScript>().Bind<::Script>();
	GetComponent<Components::NativeScript>().Bind<MoveByDragScript>();
	//AddComponent<Components::NativeScript>().Bind<MoveByDragScript>();
}


LineEntity& LineEntity::Build(Vector2 end)
{
	auto& line = GetComponent<Components::Arrow>();

	line.Origin = {0.0f, 0.0f};
	line.End = end;
	line.FillColor = BLUE;
	line.Thickness = 5.0f;
	line.EndHead = Components::Arrow::Sharp;

	return *this;
}
