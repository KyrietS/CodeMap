#pragma once

#include "ScriptableEntity.hpp"

namespace Script
{
	class ClickToAddText : public ScriptableEntity
	{
	public:
		ClickToAddText(Entity entity) : ScriptableEntity(entity) {}
		void OnUpdate() override;
	};
}
