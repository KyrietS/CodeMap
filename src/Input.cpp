#include "pch.hpp"
#include "Input.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Canvas/Canvas.hpp"

Vector2 Input::GetScreenMousePosition()
{
	return GetMousePosition();
}

Vector2 Input::GetWorldMousePosition()
{
	return Canvas::Camera().GetScreenToWorld(GetMousePosition());
}

Vector2 Input::GetWorldMousePositionRelativeTo(Vector2 origin)
{
	return Vector2Subtract(GetWorldMousePosition(), origin);
}
