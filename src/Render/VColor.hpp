#pragma once
#include <glm/vec4.hpp>


namespace VColor
{
	static constexpr glm::vec4 FromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
	}

	static constexpr glm::vec4 Black = FromBytes(0, 0, 0, 255);
	static constexpr glm::vec4 Blue = FromBytes(0, 121, 241, 255);
	static constexpr glm::vec4 Red = FromBytes(230, 41, 55, 255);
	static constexpr glm::vec4 Orange = FromBytes( 255, 161, 0, 255 );
	static constexpr glm::vec4 LightGray = FromBytes(200, 200, 200, 255);
	static constexpr glm::vec4 DarkGray = FromBytes(80, 80, 80, 255);
	static constexpr glm::vec4 RayWhite = FromBytes( 245, 245, 245, 255 );
};

