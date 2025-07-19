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
Icon LoadHandIconRGBA();
Icon LoadCursorIconRGBA();
Icon LoadTextIconRGBA();
Icon LoadArrowIconRGBA();
Icon LoadHighlightIconRGBA();

// FloppyDisk.cpp
Icon LoadFloppyDiskIconRGBA();
}
