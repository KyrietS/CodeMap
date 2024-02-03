#include "pch.hpp"
#include "AssetLoader.hpp"
#include "raylib.h"
#include "Assets/arrow_png.h"
#include "Assets/cursor_png.h"
#include "Assets/hand_png.h"
#include "Assets/highlighter_png.h"
#include "Assets/no_icon_png.h"
#include "Assets/text_png.h"

namespace Assets
{
	Icon LoadIconRGBA(const unsigned char* data, int size)
	{
	Image image = LoadImageFromMemory(".png", data, size);
	ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	std::vector<uint8_t> rgba((uint8_t*)image.data, (uint8_t*)image.data + image.width * image.height * 4);
	UnloadImage(image);
	LOG_INFO("Loaded image with size: {}x{}", image.width, image.height);
	return { rgba, image.width, image.height };
}

Icon LoadNoIconRGBA()
{
	Icon icon = LoadIconRGBA(no_icon_png, no_icon_png_size);
	LOG_INFO("Loaded no icon image with size: {}x{}", icon.Width, icon.Height);
	return icon;
}

Icon LoadHandIconRGBA()
{
	Icon icon = LoadIconRGBA(hand_png, hand_png_size);
	LOG_INFO("Loaded hand icon image with size: {}x{}", icon.Width, icon.Height);
	return icon;
}

Icon LoadCursorIconRGBA()
{
	Icon icon = LoadIconRGBA(cursor_png, cursor_png_size);
	LOG_INFO("Loaded cursor icon image with size: {}x{}", icon.Width, icon.Height);
	return icon;
}

Icon LoadTextIconRGBA()
{
	Icon icon = LoadIconRGBA(text_png, text_png_size);
	LOG_INFO("Loaded texti icon image with size: {}x{}", icon.Width, icon.Height);
	return icon;
}

Icon LoadArrowIconRGBA()
{
	Icon icon = LoadIconRGBA(arrow_png, arrow_png_size);
	LOG_INFO("Loaded arrow icon image with size: {}x{}", icon.Width, icon.Height);
	return icon;
}

Icon LoadHighlightIconRGBA()
{
	Icon icon = LoadIconRGBA(highlighter_png, highlighter_png_size);
	LOG_INFO("Loaded highlight icon image with size: {}x{}", icon.Width, icon.Height);
	return icon;
}

}