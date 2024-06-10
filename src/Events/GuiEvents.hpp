#pragma once

namespace Events::Gui
{
	struct ShowProperties
	{
		std::vector<std::uint64_t> Elements;
	};
	struct ShowAbout {};
	struct ShowPopup {};
}