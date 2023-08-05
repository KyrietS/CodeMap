#include "Canvas/Entity.hpp"
#include "Events/EventQueue.hpp"
#include "Canvas/Canvas.hpp"

Entity::Entity(const Entity& other)
	: m_EntityHandle(other.m_EntityHandle), m_Registry(other.m_Registry), m_EventQueue(other.m_EventQueue) {}
Entity::Entity(entt::entity handle)
	: Entity(handle, Canvas::Get()) {}
Entity::Entity(entt::entity handle, Canvas& canvas)
	: m_EntityHandle(handle), m_Registry(canvas.m_Registry), m_EventQueue(canvas.m_EventQueue) {}

void Entity::Destroy()
{
	m_EventQueue.Push(Events::Canvas::Destroy{ static_cast<uint32_t>(m_EntityHandle) });
}