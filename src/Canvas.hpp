#pragma once

#include "raylib.h"
#include "entt.hpp"

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
	void RemoveEntity(const entt::entity);

	template<typename Component, typename ... Other>
	static auto GetComponents()
	{
		return m_PrimaryInstance->m_Registry.view<Component, Other...>();
	};
	template<typename Component>
	static auto& GetComponent(entt::entity entity)
	{
		return m_PrimaryInstance->m_Registry.get<Component>(entity);
	}
	template<typename... Components>
	static auto& GetComponents(entt::entity entity)
	{
		return m_PrimaryInstance->m_Registry.get<Components...>(entity);
	};
	template<typename... Components>
	static bool HasComponents(entt::entity entity)
	{
		return m_PrimaryInstance->m_Registry.all_of<Components...>(entity);
	}
	static Canvas& Get() { return *m_PrimaryInstance; }
	static Camera2D& Camera() { return m_PrimaryInstance->m_Camera; }

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
