#pragma once

#include <string>

namespace Events::Canvas
{
	struct Undo {};
	struct Redo {};
	struct MakeSnapshot {};

	struct SaveToFile
	{
		SaveToFile(const std::string& filename)
			: Filename(filename) {}
		std::string Filename;
	};

	struct LoadFromFile
	{
		LoadFromFile(const std::string& filename)
			: Filename(filename) {}
		std::string Filename;
	};
}