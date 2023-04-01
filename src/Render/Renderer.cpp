#include "pch.hpp"
#include "Renderer.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "Canvas/Components.hpp"
#include <Utils/System.hpp>
#include "Fonts/Arial.h"

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

	Font GetFont()
	{
		return Renderer::s_FontData ? *((Font*)Renderer::s_FontData) : GetFontDefault();
	}

	// Helper function for development purposes
	[[maybe_unused]] void ExportFontToHeaderFile()
	{
		auto fontPath = Utils::System::GetSystemFontDirPath() + "\\arial.ttf";
		// The last polish Unicode character is 380
		std::array<int, 381> glyphs;
		for (int i = 0; i < glyphs.size(); i++)
		{
			glyphs[i] = i;
		}

		Font font = LoadFontEx(fontPath.c_str(), 64, glyphs.data(), (int)glyphs.size());
		if (not ExportFontAsCode(font, "Arial.h"))
		{
			LOG_ERROR("Failed to export the font!");
		}
	}
}

void* Renderer::s_FontData = nullptr;

void Renderer::LoadFont()
{
	Font font = LoadFont_Arial();
	SetImageFilter(font.texture.id, ImageFilter::Linear);
	Renderer::s_FontData = new Font(font);
}

void Renderer::UnloadFont()
{
	delete Renderer::s_FontData;
	s_FontData = nullptr;
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
	::DrawCircleLines(position.x, position.y, radius, vec4ToColor(color));
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

void Renderer::DrawText(const glm::vec2& position, const Components::Text& text)
{
	Vector2 textPosition{ position.x, position.y };
	Color fontColor = vec4ToColor(text.FontColor);
	::rl_DrawTextEx(GetFont(), text.Content.c_str(), textPosition, text.FontSize, text.LetterSpacing, fontColor);
}

glm::vec2 Renderer::MeasureText(const Components::Text& text)
{
	auto vec = ::MeasureTextEx(GetFont(), text.Content.c_str(), text.FontSize, text.LetterSpacing);
	return { vec.x, vec.y };
}

TextureId Renderer::LoadTextureFromBytes(std::span<uint8_t> data, int width, int height)
{
	assert(data.size() == width * height);

	void* pixelData = data.data();
	PixelFormat format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	int numOfMipmaps = 1;

	return rlLoadTexture(pixelData, width, height, format, numOfMipmaps);
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
