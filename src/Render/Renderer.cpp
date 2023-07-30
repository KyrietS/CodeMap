#include "pch.hpp"
#include "Renderer.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "Canvas/Components.hpp"
#include <Utils/System.hpp>
#include "Atlas.hpp"
#include "TextShaper.hpp"
#include "Render/Fonts/compressed_roboto.h"
#include "render/Fonts/FontStorage.hpp"
#include "Utils/Strings.hpp"

namespace
{
	Color vec4ToColor(const glm::vec4& color)
	{
		const auto r = static_cast<unsigned char>(color.r * 255);
		const auto g = static_cast<unsigned char>(color.g * 255);
		const auto b = static_cast<unsigned char>(color.b * 255);
		const auto a = static_cast<unsigned char>(color.a * 255);

		return Color{ r, g, b, a };
	}
}


FontStorage Renderer::s_FontStorage{};


void Renderer::BeginFrame()
{
	::BeginDrawing();
}

void Renderer::EndFrame()
{
	::EndDrawing();
	::SwapScreenBuffer(); // Renderer::Flush()
}

void Renderer::BeginCameraView(const CameraData& camera)
{
	Vector2 offset{ camera.offset.x, camera.offset.y };
	Vector2 target{ camera.target.x, camera.target.y };
	Camera2D camera2D{ offset, target, camera.rotation, camera.zoom };

	::BeginMode2D(camera2D);
}

void Renderer::EndCameraView()
{
	::EndMode2D();
}

void Renderer::ClearScreen(const glm::vec4& color)
{
	ClearBackground(vec4ToColor(color));
}

void Renderer::DrawLine(const glm::vec2& p0, const glm::vec2 p1, float thickness, const glm::vec4& color)
{
	Vector2 begin{ p0.x, p0.y };
	Vector2 end{ p1.x, p1.y };
	Color strokeColor = vec4ToColor(color);
	::DrawLineEx(begin, end, thickness, strokeColor);
}

void Renderer::DrawBezier(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, float thickness, const glm::vec4& color)
{
	Vector2 begin{ p0.x, p0.y };
	Vector2 end{ p2.x, p2.y };
	Vector2 controlPoint{ p1.x, p1.y };
	Color strokeColor = vec4ToColor(color);
	::DrawLineBezierQuad(begin, end, controlPoint, thickness, strokeColor);
}

void Renderer::DrawTriangle(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color)
{
	::DrawTriangle({ p0.x, p0.y }, { p1.x, p1.y }, {p2.x, p2.y}, vec4ToColor(color));
}

void Renderer::DrawRectangle(const glm::vec2& position, float width, float height, const glm::vec4& color)
{
	::DrawRectangleV({ position.x, position.y }, { width, height }, vec4ToColor(color));
}

void Renderer::DrawRectangleLines(const glm::vec2& position, float width, float height, float thickness, const glm::vec4& color)
{
	rl_Rectangle rectangle{ position.x, position.y, width, height };
	Color strokeColor = vec4ToColor(color);
	::DrawRectangleLinesEx(rectangle, thickness, strokeColor);
}

void Renderer::DrawCircleOutline(const glm::vec2& position, float radius, const glm::vec4& color)
{
	int x = (int)std::round(position.x);
	int y = (int)std::round(position.y);
	::DrawCircleLines(x, y, radius, vec4ToColor(color));
}

void Renderer::DrawImage(const glm::vec2& position, const Components::Image& image)
{
	int mipmaps = 1;
	int format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

	Texture2D texture2D = {
		*image.TextureId,
		image.Width,
		image.Height,
		mipmaps,
		format
	};
	Vector2 texturePosition{ position.x, position.y };
	Color tint = WHITE;

	::DrawTextureV(texture2D, texturePosition, tint);
}

static void DrawTextLine(const glm::vec2& position, const FontInstance& font, std::string_view line)
{
	std::string strLine{ line.begin(), line.end() };
	std::replace(strLine.begin(), strLine.end(), '\t', ' ');
	Trex::ShapedGlyphs glyphs = font.Shaper->ShapeAscii(strLine);

	glm::vec2 cursor = position;
	for (const Trex::ShapedGlyph& glyph : glyphs)
	{
		float x = cursor.x + glyph.xOffset + glyph.info.bearingX;
		float y = cursor.y + glyph.yOffset - glyph.info.bearingY;

		rl_Rectangle atlasFragment = {
				.x = (float)glyph.info.x,
				.y = (float)glyph.info.y, // top-left corner of the glyph in the atlas
				.width = (float)glyph.info.width, // width of the glyph in the atlas
				.height = (float)glyph.info.height // height of the glyph in the atlas
		};

		// Draw fragment of atlas texture
		::DrawTextureRec(*(font.Texture), atlasFragment, { x, y }, WHITE);

		cursor.x += glyph.xAdvance;
		cursor.y += glyph.yAdvance;
	}
}

void Renderer::DrawText(const glm::vec2& position, std::string_view text, float fontSize, const glm::vec4& fontColor)
{
	Renderer::DrawText(position, Components::Text{ text.data(), fontSize, 1.0f, fontColor});
}

void Renderer::DrawText(const glm::vec2& position, const Components::Text& text)
{
	// Draws text aligned to the left
	glm::vec2 cursor = position;
	auto lines = Utils::Strings::SplitToLines(text.Content);
	for (const auto& line : lines)
	{
		const FontInstance& font = Renderer::s_FontStorage.GetFont(text.FontId, text.FontSize);
		DrawTextLine(cursor, font, line);
		cursor.y += font.Shaper->GetBaselineHeight();
	}
}

static TextMeasurement MeasureMultilineText(const std::vector<Trex::TextMeasurement>& measurements, int baselineHeight)
{
	if (measurements.empty())
		return TextMeasurement{};

	auto maxAdvanceX = std::max_element(measurements.begin(), measurements.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.xAdvance < rhs.xAdvance;
	});
	auto maxWidth = std::max_element(measurements.begin(), measurements.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.width < rhs.width;
	});
	auto totalHeight = std::abs(measurements.front().yOffset) + (measurements.size() - 1) * baselineHeight + measurements.back().height - std::abs(measurements.back().yOffset);

	return {
		.Size = { maxWidth->width, totalHeight },
		.Offset = { measurements.front().xOffset, measurements.front().yOffset },
		.Advance = { maxAdvanceX->xAdvance, 0 }
	};
}

TextMeasurement Renderer::MeasureText(const Components::Text& text)
{
	auto& font = Renderer::s_FontStorage.GetFont(text.FontId, text.FontSize);
	auto lines = Utils::Strings::SplitToLines(text.Content);
	std::vector<Trex::TextMeasurement> measurements;

	// Measure each line separately
	for (auto line : lines)
	{
		std::string lineStr{ line.begin(), line.end() };
		const auto glyphs = font.Shaper->ShapeAscii(lineStr.c_str());
		measurements.push_back(font.Shaper->Measure(glyphs));
	}

	// Calculate total size
	return MeasureMultilineText(measurements, font.Shaper->GetBaselineHeight());
}

int Renderer::GetBaselineHeight(const Components::Text& text)
{
	auto& font = Renderer::s_FontStorage.GetFont(text.FontId, text.FontSize);
	return font.Shaper->GetBaselineHeight();
}

std::shared_ptr<TextureId> Renderer::LoadTextureFromBytes(std::span<uint8_t> data, int width, int height)
{
	assert(data.size() == width * height);

	void* pixelData = data.data();
	PixelFormat format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	int numOfMipmaps = 1;

	auto id = rlLoadTexture(pixelData, width, height, format, numOfMipmaps);
    std::shared_ptr<TextureId> textureId(new TextureId{ id }, [](auto* id) {
        UnloadImage( *id );
        delete id;
    });

    return textureId;
}

std::vector<uint8_t> Renderer::LoadBytesFromImage(const Components::Image& image)
{
    Texture2D texture = {
        *image.TextureId,
        image.Width,
        image.Height,
        1,
        PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    Image textureImage = LoadImageFromTexture(texture);
    if (textureImage.data == nullptr)
    {
        LOG_ERROR("Failed to load image from texture!");
        return {};
    }

    const int channels = 4;
    const int dataSize = textureImage.width * textureImage.height * channels;
    std::vector<uint8_t> data((uint8_t*)textureImage.data, (uint8_t*)textureImage.data + dataSize);

    ::UnloadImage(textureImage);

    return data;
}

void Renderer::UnloadImage(TextureId imageId)
{
	Texture texture{ imageId };
	UnloadTexture(texture);
}

void Renderer::SetImageFilter(TextureId imageId, ImageFilter imageFilter)
{
	Texture texture{ imageId };
	auto rayFilter = TEXTURE_FILTER_POINT;
	switch (imageFilter)
	{
	case ImageFilter::Nearest: rayFilter = TEXTURE_FILTER_POINT; break;
	case ImageFilter::Linear: rayFilter = TEXTURE_FILTER_BILINEAR; break;
	default:
		LOG_ERROR("Unknown image filter! Using NEAREST as default");
	}

	SetTextureFilter(texture, rayFilter);
}
