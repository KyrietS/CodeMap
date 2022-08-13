#pragma once

#include "raylib.h"
#include "entt.hpp"
#include "CanvasCamera.hpp"

struct CanvasProps
{
	Color BackgroundColor = RAYWHITE;
};

class Entity;

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

	template<typename... Components>
	static auto GetAllEntitiesWith()
	{
		return m_PrimaryInstance->m_Registry.view<Components...>();
	};

	static Canvas& Get() { return *m_PrimaryInstance; }
	static CanvasCamera& Camera() { return m_PrimaryInstance->m_Camera; }

private:
	void HandlePasteImage();
	void ScheduleEntityForDestruction(const entt::entity entity);

	// Drawing
	void DrawGrid();
	void DrawGui();

private:
	entt::registry m_Registry;
	CanvasProps m_Props;
	//Camera2D m_Camera;
	CanvasCamera m_Camera;
	std::list<entt::entity> m_ToBeRemoved;

	static Canvas* m_PrimaryInstance;

	friend class Entity;
};
