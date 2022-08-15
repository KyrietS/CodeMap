#pragma once

#include "Entity.hpp"
#include "Canvas.hpp"

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
		return m_Entity.GetComponent<T>();
	}

protected:
	Entity m_Entity;
	friend class ScriptEngine;
};
