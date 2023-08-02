#pragma once

namespace Events::Gui
{
	template<typename Component>
	struct ShowProperties
	{
		Component& Component;
	};
	struct ShowPopup {};
}