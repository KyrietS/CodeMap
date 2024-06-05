#pragma once

#include "Canvas2/Tool.hpp"

namespace Events::Canvas
{
	struct Undo {};
	struct Redo {};
	struct MakeSnapshot {};

	struct Cut {};
	struct Copy {};
	struct Paste {};

	struct SetFocus
	{
		std::uint32_t EntityId; // TODO: In the future use UUID
	};

	struct ClearFocus {};

	struct Destroy
	{
		std::uint32_t EntityId;
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