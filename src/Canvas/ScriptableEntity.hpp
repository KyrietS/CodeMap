#pragma once

#include "Entity.hpp"

class ScriptableEntity
{
public:
	ScriptableEntity() = default;
	virtual ~ScriptableEntity() = default;

	virtual void OnCreate() {}
	virtual void OnUpdate() {}
	virtual void OnDestroy() {}

	template<typename T>
	T& GetComponent()
	{
		return m_Entity->GetComponent<T>();
	}

protected:
	std::unique_ptr<Entity> m_Entity; // Initialized by ScriptEngine
	friend class ScriptEngine;
};
