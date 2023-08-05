#pragma once

#include "entt/entt.hpp"
#include "Components.hpp"


class Canvas;
class EventQueue;

class Entity
{
public:
	Entity(const Entity&);
	Entity(entt::entity handle);
	Entity(entt::entity handle, Canvas& canvas);

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		assert(!HasComponent<T>());
		T& component = m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename T, typename... Args>
	void AttachScript(Args&&... args)
	{
		if (HasComponent<Components::NativeScript>())
		{
			GetComponent<Components::NativeScript>().Bind<T>(std::forward<Args>(args)...);
		}
		else
		{
			AddComponent<Components::NativeScript>().Bind<T>(std::forward<Args>(args)...);
		}
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& component = m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	void AddChild(Entity child)
	{
		assert(HasComponent<Components::Hierarchy>());
		assert(child.HasComponent<Components::Hierarchy>());
		assert(child.GetComponent<Components::Hierarchy>().Parent == entt::null);

		auto& childHierarchy = child.GetComponent<Components::Hierarchy>();
		auto& parentHierarchy = GetComponent<Components::Hierarchy>();
		parentHierarchy.Children.push_back(child);
		childHierarchy.Parent = *this;
	}

	template<typename T>
	T& GetComponent() const
	{
		assert(HasComponent<T>());
		return m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent() const
	{
		return m_Registry.any_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_Registry.remove<T>(m_EntityHandle);
	}

	void Destroy();

	bool IsValid() const { return m_Registry.valid(m_EntityHandle); }
	operator bool() const { return IsValid(); }
	operator entt::entity() const { return m_EntityHandle; }
	operator uint32_t() const { return (uint32_t)m_EntityHandle; }

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

protected:
	entt::entity m_EntityHandle = entt::null;
	entt::registry& m_Registry;
	EventQueue& m_EventQueue;
};
