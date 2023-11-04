#pragma once

namespace Assets
{
struct Icon
{
	std::vector<uint8_t> RGBA;
	int Width;
	int Height;
};

Icon LoadNoIconRGBA();
}
