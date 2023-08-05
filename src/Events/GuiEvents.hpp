#pragma once
#include "Canvas/Entity.hpp"

namespace Events::Gui
{
	struct ShowProperties
	{
		Entity Entity;
	};
	struct ShowPopup {};
}