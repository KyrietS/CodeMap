#pragma once

#include "entt/entt.hpp"
#include "CanvasCamera.hpp"
#include <glm/vec2.hpp>
#include "Render/VColor.hpp"
#include "Controllers/IController.hpp"
#include "Serializer/CanvasSerializer.hpp"
#include "Deserializer/CanvasDeserializer.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas/Entity.hpp"

struct CanvasProps
{
	glm::vec4 BackgroundColor = VColor::RayWhite;
};

class Canvas
{
public:
	Canvas(EventQueue&, bool primary = true);
	~Canvas();

	void Draw();
	void OnUpdate();
	void OnEvent(Event&);

	// Move this function to some Factory class
	// To pass it around and make it possible for controllers
	// to create their own entities.
	Entity CreateEntity(glm::vec2 = { 0, 0 });

	template<typename... Components>
	static auto GetAllEntitiesWith()
	{
		return m_PrimaryInstance->m_Registry.view<Components...>();
	};

	static Canvas& Get() { return *m_PrimaryInstance; }
	static CanvasCamera& Camera() { return m_PrimaryInstance->m_Camera; }

private:
	void OnDestroy(const Events::Canvas::Destroy&);
	void OnCanvasSaveToFile(const Events::Canvas::SaveToFile&);
	void OnCanvasLoadFromFile(const Events::Canvas::LoadFromFile&);

	Entity CreateVoidEntity();
	void ScheduleEntityForDestruction(const entt::entity entity);
	void UpdateHierarchy();
	void UpdateChildrenOf(Entity parent);

	void DrawGrid();

private:
	entt::registry m_Registry;
	CanvasProps m_Props;
	CanvasCamera m_Camera;
	EventQueue& m_EventQueue;

	std::vector<std::unique_ptr<IController>> m_Controllers;

#ifdef NDEBUG
	bool m_DebugMode = false;
#else
	bool m_DebugMode = true;
#endif

	static Canvas* m_PrimaryInstance;

	friend class Entity;
};
