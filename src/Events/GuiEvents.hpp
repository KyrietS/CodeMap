#pragma once
#include "Canvas/Entity.hpp"

namespace Events::Gui
{
	struct ShowProperties
	{
		Entity Entity;
	};
	struct ShowProperties2
	{
		std::uint64_t ElementId; // TODO: vector
	};
	struct ShowPopup {};
}