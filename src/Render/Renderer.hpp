#pragma once

#include "CameraData.hpp"
#include <glm/glm.hpp>
#include <span>

// <Windows.h> I hate you so much!!!
#undef DrawText

namespace Components
{
struct Text;
struct Image;
}

enum class ImageFilter
{
	Nearest,
	Linear
};
using TextureId = unsigned int;

class Renderer
{
public:
	static void BeginFrame();
	static void EndFrame();

	static void BeginCameraView(const CameraData&);
	static void EndCameraView();

	static void ClearScreen(const glm::vec4 & color);

	static void DrawLine(const glm::vec2& p0, const glm::vec2 p1, float thickness, const glm::vec4& color);
	static void DrawBezier(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, float thickness, const glm::vec4& color);

	static void DrawTriangle(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color);
	static void DrawRectangle(const glm::vec2& position, float width, float height, const glm::vec4& color);
	static void DrawRectangleLines(const glm::vec2& position, float width, float height, float thickness, const glm::vec4& color);
	static void DrawCircleOutline(const glm::vec2& position, float radius, const glm::vec4& color);
	static void DrawImage(const glm::vec2& position, const Components::Image& image);
	static void DrawText(const glm::vec2& position, std::string_view text, float fontSize, const glm::vec4& fontColor);
	static void DrawText(const glm::vec2& position, const Components::Text& text);
    static void LoadFontAtlas();
    static void UnloadFontAtlas();

	static glm::vec2 MeasureText(const Components::Text& text);
    // Loads texture from RGBA|RGBA|RGBA|... data.
	// One color component is represented by 1 byte.
	static TextureId LoadTextureFromBytes(std::span<uint8_t> data, int width, int height);
    static void UnloadImage(TextureId);

	static void SetImageFilter(TextureId, ImageFilter);
    static void* s_FontAtlas;
    static void* s_TextShaper;
    static void* s_FontTexture;
};

