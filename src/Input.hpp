#pragma once

#include "raylib.h"

// TODO: Implement IsMouseButtonPressed(MouseCode button);
// TODO: Implement IsKeyPressed(KeyCode key); // as well as *Released, *Down, *Up
// TODO: Implement BeginTextMode(), EndTextMode(), GetText()
// TODO: Use this class for inputs instead of raylib everywhere

class Input
{
public:
	static Vector2 GetScreenMousePosition();
	static Vector2 GetWorldMousePosition();
	static Vector2 GetWorldMousePositionRelativeTo(Vector2 origin);
};

