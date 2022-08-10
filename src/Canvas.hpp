#pragma once

#include "raylib.h"
#include "entt.hpp"
#include "Entity.hpp"

struct CanvasProps
{
	Color BackgroundColor = RAYWHITE;
};

class Canvas
{
public:
	Canvas(bool primary = true);
	~Canvas();
	// TODO: Draw() and OnUpdate() should be the same function.
	//       This separation is very artificial and doesn't make much sense.
	void Draw();
	void OnUpdate();
	Entity CreateEntity(Vector2 = { 0, 0 });

	void RemoveEntity(const entt::entity);
	static Canvas& Get() { return *m_PrimaryInstance; }
	static Camera2D& Camera() { return Get().m_Camera; }

private:
	void HandlePasteImage();

	// Drawing
	void DrawGrid();
	void DrawGui();

private:
	entt::registry m_Registry;
	CanvasProps m_Props;
	Camera2D m_Camera;
	std::list<entt::entity> m_ToBeRemoved;

	static Canvas* m_PrimaryInstance;

	friend class Entity;
};
