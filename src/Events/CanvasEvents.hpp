#pragma once

#include "Canvas/Tool.hpp"

namespace Events::Canvas
{
	struct Undo {};
	struct Redo {};
	struct MakeSnapshot {};

	struct Cut {};
	struct Copy {};
	struct Paste {};

	struct SelectElement
	{
		std::uint64_t ElementId;
		bool MultiSelect = false;
	};

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

	struct SelectTool
	{
		ToolType Tool;
	};
}