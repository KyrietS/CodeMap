#include "pch.hpp"
#include "Window.hpp"
#include "raylib.h"


void Window::Init(uint32_t width, uint32_t height, const std::string& title)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(width, height, title.c_str());
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);
}

uint32_t Window::GetWidth()
{
	return (uint32_t)GetScreenWidth();
}

uint32_t Window::GetHeight()
{
	return (uint32_t)GetScreenHeight();
}

void Window::Close()
{
	CloseWindow();
}

bool Window::ShouldClose()
{
	return WindowShouldClose();
}
