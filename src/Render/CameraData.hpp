#pragma once
#include <glm/vec2.hpp>

struct CameraData
{
	glm::vec2 offset;         // Camera offset (displacement from target)
	glm::vec2 target;         // Camera target (rotation and zoom origin)
	float rotation;           // Camera rotation in degrees
	float zoom;               // Camera zoom (scaling), should be 1.0f by default
};
