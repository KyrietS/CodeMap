#pragma once

#include "entt/entt.hpp"
#include "CanvasCamera.hpp"
#include <glm/vec2.hpp>
#include "Render/VColor.hpp"
#include "Controllers/IController.hpp"
#include "Serializer/CanvasSerializer.hpp"
#include "Deserializer/CanvasDeserializer.hpp"

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

	void Draw();
	void OnUpdate();
	void OnEvent(Event&);

	// Move this function to some Factory class
	// To pass it around and make it possible for controllers
	// to create their own entities.
	Entity CreateEntity(glm::vec2 = { 0, 0 });
	std::unique_ptr<CanvasSerializer> GetSerializer();
	std::unique_ptr<CanvasDeserializer> GetDeserializer();

	template<typename... Components>
	static auto GetAllEntitiesWith()
	{
		return m_PrimaryInstance->m_Registry.view<Components...>();
	};

	static Canvas& Get() { return *m_PrimaryInstance; }
	static CanvasCamera& Camera() { return m_PrimaryInstance->m_Camera; }

private:
	Entity CreateVoidEntity();
	void ScheduleEntityForDestruction(const entt::entity entity);
	void UpdateHierarchy();
	void UpdateChildrenOf(Entity parent);

	void DrawGrid();

private:
	entt::registry m_Registry;
	CanvasProps m_Props;
	CanvasCamera m_Camera;

	std::vector<std::unique_ptr<IController>> m_Controllers;
	std::list<entt::entity> m_ToBeRemoved;

	bool m_DebugMode = true;

	static Canvas* m_PrimaryInstance;

	friend class Entity;
};
