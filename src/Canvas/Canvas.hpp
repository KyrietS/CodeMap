#pragma once

#include "entt.hpp"
#include "CanvasCamera.hpp"
#include <glm/vec2.hpp>
#include "Render/VColor.hpp"

struct CanvasProps
{
	glm::vec4 BackgroundColor = VColor::RayWhite;
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

	Entity CreateEntity(glm::vec2 = { 0, 0 });

	template<typename... Components>
	static auto GetAllEntitiesWith()
	{
		return m_PrimaryInstance->m_Registry.view<Components...>();
	};

	static Canvas& Get() { return *m_PrimaryInstance; }
	static CanvasCamera& Camera() { return m_PrimaryInstance->m_Camera; }

private:
	Entity CreateVoidEntity();
	void HandlePasteImage();
	void ScheduleEntityForDestruction(const entt::entity entity);

	// Drawing
	void DrawGrid();
	void DrawGui();

private:


	entt::registry m_Registry;
	CanvasProps m_Props;
	CanvasCamera m_Camera;
	std::list<entt::entity> m_ToBeRemoved;

	bool m_DebugMode = true;

	static Canvas* m_PrimaryInstance;

	friend class Entity;
};
