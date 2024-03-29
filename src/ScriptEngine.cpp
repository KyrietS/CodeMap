#include "pch.hpp"
#include "ScriptEngine.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/ScriptableEntity.hpp"


ScriptEngine::ScriptEngine(Canvas& canvas)
	: m_Canvas(canvas)
{

}

void ScriptEngine::OnScriptsUpdate()
{
	auto view = m_Canvas.GetAllEntitiesWith<Components::NativeScript>();
	for (auto [entity, scripts] : view.each())
	{
		for (auto& script : scripts.Instances)
		{
			if (!script)
			{
				script.m_Instance = script.Instantiate();
				script.m_Instance->m_Entity = std::make_unique<Entity>(entity, m_Canvas);
				script->OnCreate();
			}
			script->OnUpdate();
		}
	}
}
