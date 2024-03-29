#pragma once

#include "CameraData.hpp"
#include "TextMeasurement.hpp"
#include "BlendMode.hpp"
#include "Render/Fonts/FontStorage.hpp"
#include <glm/glm.hpp>
#include <span>
#include <vector>

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

	static void BeginBlendMode(Render::BlendMode);
	static void EndBlendMode();

	static void ClearScreen(const glm::vec4 & color);

	static void DrawLine(const glm::vec2& p0, const glm::vec2 p1, float thickness, const glm::vec4& color);
	static void DrawBezier(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, float thickness, const glm::vec4& color);

	static void DrawTriangle(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color);
	static void DrawRectangle(const glm::vec2& position, float width, float height, const glm::vec4& color);
	static void DrawRectangleLines(const glm::vec2& position, float width, float height, float thickness, const glm::vec4& color);
	static void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);
	static void DrawCircleOutline(const glm::vec2& position, float radius, const glm::vec4& color);
	static void DrawPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color);
	static void DrawImage(const glm::vec2& position, const Components::Image& image);
	static void DrawText(const glm::vec2& position, const Components::Text& text);

	static TextMeasurement MeasureText(const Components::Text& text);
	static int GetBaselineHeight(const Components::Text& text);
	// Loads texture from RGBA|RGBA|RGBA|... data.
	// One color component is represented by 1 byte.
	static std::shared_ptr<TextureId> LoadTextureFromBytes(std::span<const uint8_t> data, int width, int height);
	// Loads image bitmap in RGBA|RGBA|RGBA|... format from the GPU.
	static std::vector<uint8_t> LoadBytesFromImage(const Components::Image&);
	static void SetImageFilter(TextureId, ImageFilter);

private:
	static void UnloadImage(TextureId);
	static FontStorage s_FontStorage;
};

