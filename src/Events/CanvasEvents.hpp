#pragma once

#include <string>

namespace Events::Canvas
{
	struct SaveToFile
	{
		std::string Filename;
	};

	struct LoadFromFile
	{
		std::string Filename;
	};
}