#include "pch.hpp"
#include "Cursor.hpp"
#include "external/glfw/include/GLFW/glfw3.h"

namespace
{
	// Symbol to pixel mapping:
	// ' ' - transparent
	// '#' - black
	// ',' - white
	// 'o' - orange
	// 'x' - blue
	static const unsigned char arrowCursorPixels[] = {
		"   ,,,              "
		"   ,#,              "
		"   ,#,              "
		",,,,#,,,,           "
		",#######,           "
		",,,,#,,,,           "
		"   ,#,,,,           "
		"   ,#,,#,,          "
		"   ,,,,##,,         "
		"      ,#o#,,        "
		"      ,#oo#,,       "
		"      ,#ooo#,,      "
		"      ,#oooo#,,     "
		"      ,#ooooo#,,    "
		"      ,#oooooo#,,   "
		"      ,#ooooooo#,,  "
		"      ,#oooooooo#,, "
		"      ,#ooooooooo#,,"
		"      ,#oooooo#####,"
		"      ,#ooo#ox#,,,,,"
		"      ,#oo##xx#,,   "
		"      ,#o#,,#xx#,   "
		"      ,##,,,#xx#,,  "
		"      ,#,, ,,#xx#,  "
		"      ,,,   ,#xx#,, "
		"            ,,#xx#, "
		"             ,#xx#, "
		"             ,,##,, "
		"              ,,,,  "
	};
	static const int arrowCursorWidth = 20;
	static const int arrowCursorHeight = 29;

	std::vector<unsigned char> LoadCursorImageRGBA()
	{
		size_t mappingSize = arrowCursorWidth * arrowCursorHeight;
		std::vector<unsigned char> pixels(mappingSize * 4);

		for (size_t i = 0; i < mappingSize; i++)
		{
			unsigned char symbol = arrowCursorPixels[i];
			unsigned char color[ 4 ] = {};

			switch (symbol)
			{
			case ' ': // transparent
				color[ 0 ] = 0;
				color[ 1 ] = 0;
				color[ 2 ] = 0;
				color[ 3 ] = 0;
				break;
			case '#': // black
				color[ 0 ] = 0;
				color[ 1 ] = 0;
				color[ 2 ] = 0;
				color[ 3 ] = 255;
				break;
			case ',': // white
				color[ 0 ] = 255;
				color[ 1 ] = 255;
				color[ 2 ] = 255;
				color[ 3 ] = 128;
				break;
			case 'o': // Orange
				color[ 0 ] = 255;
				color[ 1 ] = 161;
				color[ 2 ] = 0;
				color[ 3 ] = 255;
				break;
			case 'x': // Blue
				color[ 0 ] = 0;
				color[ 1 ] = 121;
				color[ 2 ] = 241;
				color[ 3 ] = 255;
				break;
			default:
				assert(false);
			}

			pixels[ i * 4 + 0 ] = color[ 0 ];
			pixels[ i * 4 + 1 ] = color[ 1 ];
			pixels[ i * 4 + 2 ] = color[ 2 ];
			pixels[ i * 4 + 3 ] = color[ 3 ];
		}

		return pixels;
	}

	static GLFWcursor* arrowCursor;
}

Cursor::Cursor(void* window)
	: m_Window(window)
{
	auto pixels = LoadCursorImageRGBA();
	GLFWimage cursorImage = { arrowCursorWidth, arrowCursorHeight, pixels.data()};
	arrowCursor = glfwCreateCursor(&cursorImage, 4, 4);
	assert(arrowCursor);
}

void Cursor::Lock(bool status)
{
	m_Locked = status;
}

void Cursor::Reset()
{
	if (m_Locked)
		return;
	glfwSetCursor((GLFWwindow*)m_Window, nullptr);
}

void Cursor::SetArrowImage()
{
	if (m_Locked)
		return;

	glfwSetCursor((GLFWwindow*)m_Window, arrowCursor);
}