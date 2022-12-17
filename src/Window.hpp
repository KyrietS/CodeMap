#pragma once
#include <string>

class Window
{
public:
	static void Init(uint32_t width, uint32_t height, const std::string& title);
	static uint32_t GetWidth();
	static uint32_t GetHeight();

	static void Close();
	static bool ShouldClose(); // Close icon clicked
};

