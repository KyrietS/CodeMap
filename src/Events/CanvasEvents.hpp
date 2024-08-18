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

	struct PasteText
	{
		std::string Text;
	};

	struct SaveToFile
	{
		bool SaveAs = false;
	};

	struct LoadFromFile {};

	struct SelectTool
	{
		ToolType Tool;
	};
}