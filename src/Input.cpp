#include "pch.hpp"
#include "Input.hpp"
#include "raylib.h"
#include "Canvas/Canvas.hpp"

Vector2 Input::GetScreenMousePosition()
{
	return GetMousePosition();
}

Vector2 Input::GetWorldMousePosition()
{
	return Canvas::Camera().GetScreenToWorld(GetMousePosition());
}
