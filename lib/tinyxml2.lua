project "tinyxml2"
	location ""
	language "C++"
	cppdialect "C++17"
	targetname "tinyxml2"
	kind "StaticLib"
	warnings "Off"

	files {
		"tinyxml2/tinyxml2.cpp",
		"tinyxml2/tinyxml2.h"
	}
