#pragma once

#include "Canvas/ScriptableEntity.hpp"
#include <glm/vec2.hpp>

class MoveByDragScript : public ScriptableEntity
{
public:
	MoveByDragScript(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnCreate() override;
	void OnUpdate() override;

private:
	void MoveBy(glm::vec2 positionChange);

private:
	bool m_CurrentlyMoving = false;
	EventQueue& m_EventQueue;
};

