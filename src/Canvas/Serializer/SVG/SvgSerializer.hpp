#pragma once
#include "entt/entt.hpp"
#include <glm/vec2.hpp>
#include "Canvas/Serializer/CanvasSerializer.hpp"
#include "Canvas/Entity.hpp"
#include "Canvas/Components.hpp"
#include <tinyxml2.h>
#include "ViewBox.hpp"


class SvgSerializer : public CanvasSerializer
{
public:
	SvgSerializer(const entt::registry&);
	std::string Serialize() override;

private:
	void GenerateSvgRootElement(tinyxml2::XMLDocument&, ViewBox);
	ViewBox GetCanvasViewBox();
	void UpdateCanvasViewBoxFromFocusArea(ViewBoxBuilder& viewBox, const Entity);

	void AddBorderToSvgDocument(tinyxml2::XMLElement& root, ViewBox);
	void AddArrowheadMarkers(tinyxml2::XMLElement& root);
    void AddRobotoFont(tinyxml2::XMLElement& root);
	void SerializeAllEntities(tinyxml2::XMLElement& root);

	void SerializeArrow(tinyxml2::XMLElement& root, const Entity entity);
	void SerializeText(tinyxml2::XMLElement& root, const Entity entity);
	void SerializeTextContent(tinyxml2::XMLElement& textElement, const Components::Text&);
    void SerializeImage(tinyxml2::XMLElement& root, const Entity entity);

	const entt::registry& m_Registry;

	static constexpr int PADDING_SIZE = 20;
};