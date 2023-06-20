#pragma once
#include <glm/vec2.hpp>

struct TextMeasurement
{
	glm::vec2 Size;    // Width and height of the text. Measured from the top-left corner (offset)
	glm::vec2 Offset;  // Offset from baseline origin to left/top
	glm::vec2 Advance; // Advance from baseline origin to the end of the text (including trailing advance)
};
