#include "pch.hpp"
#include "ScriptEngine.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"


ScriptEngine::ScriptEngine(Canvas& canvas)
	: m_Canvas(canvas)
{

}

void ScriptEngine::OnScriptsUpdate()
{
	auto view = m_Canvas.GetAllEntitiesWith<Components::NativeScript>();
	for (auto [entity, script] : view.each())
	{
		if (!script.Active)
			continue;
		if (!script.Instance)
		{
			script.Instance = script.Instantiate();
			print("Instantiating script: {}", typeid(*(script.Instance)).name());
			script.Instance->m_Entity = { entity, &m_Canvas };
			script.Instance->OnCreate();
		}
		script.Instance->OnUpdate();
	}
}
