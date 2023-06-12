#include "pch.hpp"
#include "Renderer.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "Canvas/Components.hpp"
#include <Utils/System.hpp>
#include "Atlas.hpp"
#include "TextShaper.hpp"

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

    Trex::Atlas& GetFontAtlas()
    {
        assert(Renderer::s_FontAtlas != nullptr);
        return *(Trex::Atlas*)Renderer::s_FontAtlas;
    }

    Trex::TextShaper& GetTextShaper()
    {
        assert(Renderer::s_TextShaper != nullptr);
        return *(Trex::TextShaper*)Renderer::s_TextShaper;
    }

    Texture2D& GetFontTexture()
    {
        assert(Renderer::s_FontTexture != nullptr);
        return *(Texture2D*)Renderer::s_FontTexture;
    }

    Image GetAtlasAsBitmapImage()
    {
        auto& atlas = GetFontAtlas();
        Image atlasImage;
        atlasImage.data = atlas.GetBitmap().data(); // pointer to the atlas bitmap data
        atlasImage.width = atlas.GetWidth(); // width of the atlas bitmap
        atlasImage.height = atlas.GetHeight(); // height of the atlas bitmap
        atlasImage.mipmaps = 1;
        atlasImage.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE; // atlas bitmap format is always 1 byte per pixel (grayscale)
        return atlasImage;
    }

	// Helper function for development purposes
	[[maybe_unused]] void ExportFontToHeaderFile()
	{
		auto fontPath = "roboto.ttf";

        unsigned int fontDataSize;
        unsigned char* fontData = ::LoadFileData("roboto.ttf", &fontDataSize);
        int compressedFontDataSize;
        unsigned char* compressedFontData = ::CompressData(fontData, (int)fontDataSize, &compressedFontDataSize);
        ::SaveFileData("compressed_font.ttf", compressedFontData, compressedFontDataSize);
        ::UnloadFileData(fontData);
        ::MemFree(compressedFontData);
	}
}

void* Renderer::s_FontAtlas = nullptr;
void* Renderer::s_TextShaper = nullptr;
void* Renderer::s_FontTexture = nullptr;


void Renderer::LoadFontAtlas()
{
    const int fontSize = 32;
    const auto charset = Trex::Charset::Ascii();
    // FIXME: This will not work on Linux. Use embedded Roboto font instead.
    const auto fontPath = Utils::System::GetSystemFontDirPath() + "\\arial.ttf";
    LOG_DEBUG("Loading font atlas from: {}, size: {}", fontPath, fontSize);

    Renderer::s_FontAtlas = new Trex::Atlas(fontPath, fontSize, charset);
    LOG_DEBUG("Font atlas loaded. Size: {}x{}", GetFontAtlas().GetWidth(), GetFontAtlas().GetHeight());
    Renderer::s_TextShaper = new Trex::TextShaper(GetFontAtlas());
    LOG_DEBUG("Text shaper created.");
    Renderer::s_FontTexture = new Texture2D(LoadTextureFromImage(GetAtlasAsBitmapImage()));
    SetTextureFilter(GetFontTexture(), TEXTURE_FILTER_BILINEAR);

    // TODO: I can remove atlas from memory now. Shaper has a copy of it.
}

void Renderer::UnloadFontAtlas()
{
    delete (Trex::Atlas*)Renderer::s_FontAtlas;
    delete (Trex::TextShaper*)Renderer::s_TextShaper;
    delete (Texture2D*)Renderer::s_FontTexture;
    Renderer::s_FontAtlas = nullptr;
    Renderer::s_TextShaper = nullptr;
    Renderer::s_FontTexture = nullptr;
    LOG_DEBUG("Font atlas unloaded.");
}

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
		image.TextureId,
		image.Width,
		image.Height,
		mipmaps,
		format
	};
	Vector2 texturePosition{ position.x, position.y };
	Color tint = WHITE;

	::DrawTextureV(texture2D, texturePosition, tint);
}

void Renderer::DrawText(const glm::vec2& position, std::string_view text, float fontSize, const glm::vec4& fontColor)
{
	Renderer::DrawText(position, Components::Text{ text.data(), fontSize, 1.0f, fontColor});
}

void DrawTextWithAtlas(const char* text, const glm::vec2& position)
{
    Trex::TextShaper& shaper = GetTextShaper();
    Trex::ShapedGlyphs glyphs = shaper.ShapeAscii(text);

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
        ::DrawTextureRec(GetFontTexture(), atlasFragment, { x, y }, WHITE);

        cursor.x += glyph.xAdvance;
        cursor.y += glyph.yAdvance;
    }
}

void Renderer::DrawText(const glm::vec2& position, const Components::Text& text)
{
    float textHeight = MeasureText(text).y;
    DrawTextWithAtlas(text.Content.c_str(), {position.x, position.y + textHeight});
}

// FIXME: Use Trex::TextShaper to measure text
glm::vec2 Renderer::MeasureText(const Components::Text& text)
{
    const auto& glyphs = GetTextShaper().ShapeAscii(text.Content.c_str());
    glm::vec2 size = {0, 0};
    for (const auto& glyph : glyphs)
    {
        size.x += glyph.xAdvance;
        size.y = std::max(size.y, (float)glyph.info.height);
    }
    return size;
}

TextureId Renderer::LoadTextureFromBytes(std::span<uint8_t> data, int width, int height)
{
	assert(data.size() == width * height);

	void* pixelData = data.data();
	PixelFormat format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	int numOfMipmaps = 1;

	return rlLoadTexture(pixelData, width, height, format, numOfMipmaps);
}

std::vector<uint8_t> Renderer::LoadBytesFromImage(const Components::Image& image)
{
    Texture2D texture = {
        image.TextureId,
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
