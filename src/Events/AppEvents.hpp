#pragma once

namespace Events::App
{
	struct Quit {};

	struct ProjectUnsaved {};
	struct ProjectSaved
	{
		std::string ProjectName;
	};
}