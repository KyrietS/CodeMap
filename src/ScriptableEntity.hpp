#pragma once

#include "Entity.hpp"
#include "Canvas.hpp"

class ScriptableEntity
{
public:
	ScriptableEntity() = default;

	// TODO: Remove this constructor. Every script should be
	//       instantiated by Canvas / App in OnUpdate() function.
	//       When script is instantiated m_Entity should be set
	//       accordingly and OnCreate() function should be called.
	ScriptableEntity(Entity entity)
		: m_Entity(entity) {}
	virtual ~ScriptableEntity() = default;

	virtual void OnCreate() {}
	virtual void OnUpdate() {}
	virtual void OnDestroy() {}

	template<typename T>
	T& GetComponent()
	{
		return m_Entity.GetComponent<T>();
	}

private:
	Entity m_Entity;
};
