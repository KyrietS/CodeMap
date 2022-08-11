#pragma once

#include "ScriptableEntity.hpp"

namespace Script
{
	class ClickToAddText : public ScriptableEntity
	{
	public:
		void OnUpdate() override;
	};
}
