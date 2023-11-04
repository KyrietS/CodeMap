#include "pch.hpp"
#include "AssetLoader.hpp"
#include "raylib.h"
#include "Assets/compressed_no_icon.h"

namespace Assets
{
Icon LoadNoIconRGBA()
{
	int no_icon_png_len = 0;
	unsigned char* no_icon_png = DecompressData(compressed_no_icon_png, compressed_no_icon_png_len, &no_icon_png_len);

	Image image = LoadImageFromMemory(".png", no_icon_png, no_icon_png_len);
	MemFree(no_icon_png);

	ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	std::vector<uint8_t> no_icon_rgba((uint8_t*)image.data, (uint8_t*)image.data + image.width * image.height * 4);
	UnloadImage(image);


	return { no_icon_rgba, image.width, image.height };
}
}