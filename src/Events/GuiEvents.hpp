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
		std::vector<std::uint64_t> Elements;
	};
	struct ShowPopup {};
}