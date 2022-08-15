#pragma once

#include "Canvas/ScriptableEntity.hpp"

class MoveByDragScript : public ScriptableEntity
{
public:
	void OnCreate() override;
	void OnUpdate() override;

private:
	void MoveBy(Vector2 positionChange);

private:
	bool m_CurrentlyMoving = false;
};

