#pragma once

#include "Canvas/ScriptableEntity.hpp"
#include "Canvas/Components.hpp"


struct SelectionScript : ScriptableEntity
{
	void OnUpdate() override;
};