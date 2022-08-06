#include "Canvas.hpp"
#include "raylib.h"
#include "raymath.h"
#include "clip.h"

#include <cmath>
#include <iostream> // temp
#include <string>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const Vector2 v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

Canvas::Canvas() : m_Props{}
{
	m_Camera.offset = { 0, 0 };
	m_Camera.target = { 0, 0 };
	m_Camera.rotation = 0.0f;
	m_Camera.zoom = 1.0f;
}


void Canvas::Draw()
{
	BeginDrawing();
	ClearBackground(m_Props.BackgroundColor);
	{
		// Canvas world drawing
		BeginMode2D(m_Camera);
		{
			DrawGrid();
			DrawImages();
		}
		EndMode2D();

		// Screen absolute drawing
		DrawGui();
	}
	EndDrawing();
}

void Canvas::OnUpdate()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		SelectImageByScreenPos(GetMousePosition());
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
	{
		MoveCanvasWithMouse();
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		MoveObjectOnCanvasWithMouse();
	}
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V))
	{
		HandlePasteImage();
	}
	ZoomCanvas(GetMouseWheelMove());
	if (IsKeyPressed(KEY_SPACE))
	{
		//std::reverse(m_Images.begin(), m_Images.end());
		std::swap(m_Images[0], m_Images[1]);
		//m_Images.clear();
	}
}


void Canvas::MoveCanvasWithMouse()
{
	Vector2 delta = GetMouseDelta();
	delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);
	m_Camera.target = Vector2Add(m_Camera.target, delta);
}

void Canvas::ZoomCanvas(float zoomChange)
{
	if (zoomChange != 0)
	{
		Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), m_Camera);
		m_Camera.offset = GetMousePosition();
		m_Camera.target = mouseWorldPos;

		const float zoomFactor = 1.1f;
		m_Camera.zoom *= zoomChange > 0 ? zoomFactor : (1.0f / zoomFactor);

		// Limits
		m_Camera.zoom = Clamp(m_Camera.zoom, 0.1f, 10.0f);

		// Snap to 1.0
		if (m_Camera.zoom > 0.95f && m_Camera.zoom < 1.05f)
		{
			m_Camera.zoom = 1.0f;
		}

		// Filter corrections. TODO: Entity Component System: GetAll<Texture>
		for (auto& image : m_Images)
		{
			image.AdjustFilterToZoomLevel(m_Camera.zoom);
		}
	}
}

void TransformFromBgraToRgba(uint8_t* data, int size)
{
	for (int i = 0; i < size / 4; i++)
	{
		// Swap first and third byte in pixel.
		std::swap(data[i * 4 + 0], data[i * 4 + 2]);
	}
}

void Canvas::HandlePasteImage()
{
	clip::image clipboardImage;
	if (!clip::get_image(clipboardImage))
	{
		return TraceLog(LOG_WARNING, "CLIPBOARD: Failed to paste an image from clipboard");
	}

	clip::image_spec imageSpec = clipboardImage.spec();
	int size = (int)imageSpec.required_data_size();
	uint8_t* data = reinterpret_cast<uint8_t*>(clipboardImage.data());
	TransformFromBgraToRgba(data, size);
	Vector2 imagePos = GetScreenToWorld2D(GetMousePosition(), m_Camera);

	m_Images.emplace_back(imagePos, data, imageSpec.width, imageSpec.height);
	m_Images.back().AdjustFilterToZoomLevel(m_Camera.zoom);
	m_Images.back().Focus();
}

void Canvas::DrawGrid()
{
	const int DOT_GAP_SIZE = 50;
	const int NUM_OF_DOTS_HORIZONTAL = static_cast<int>((GetScreenWidth() / m_Camera.zoom) / DOT_GAP_SIZE + 2);
	const int NUM_OF_DOTS_VERTICAL = static_cast<int>((GetScreenHeight() / m_Camera.zoom) / DOT_GAP_SIZE + 2);
	float DOT_SIZE = 4;
	if (m_Camera.zoom < 1.0f)
	{
		DOT_SIZE *= (2.0f - m_Camera.zoom);
	}

	Vector2 dotScreenPos = { 0, 0 };
	Vector2 dotWorldPos = GetScreenToWorld2D(dotScreenPos, m_Camera);
	Vector2 firstDotScreenPos = {
		DOT_GAP_SIZE * ceil(dotWorldPos.x / DOT_GAP_SIZE) - DOT_GAP_SIZE,
		DOT_GAP_SIZE * ceil(dotWorldPos.y / DOT_GAP_SIZE) - DOT_GAP_SIZE
	};

	for (int i = 0; i < NUM_OF_DOTS_VERTICAL; i++)
	{
		float dotY = firstDotScreenPos.y + (i * DOT_GAP_SIZE);
		for (int k = 0; k < NUM_OF_DOTS_HORIZONTAL; k++)
		{
			float dotX = firstDotScreenPos.x + (k * DOT_GAP_SIZE);
			DrawRectangleV({ dotX, dotY }, { DOT_SIZE, DOT_SIZE }, LIGHTGRAY);
		}
	}
}

void Canvas::DrawGui()
{
	// Draw zoom level
	std::string zoomLevelText = "zoom: " + std::to_string(int(m_Camera.zoom * 100)) + "%";
	DrawText(zoomLevelText.c_str(), 30, GetScreenHeight() - 30, 10, DARKGRAY);
}

void Canvas::MoveObjectOnCanvasWithMouse()
{
	if (m_SelectedImageIndex == -1) return;

	// Scale screen distance to world distance
	Vector2 delta = Vector2Scale(GetMouseDelta(), 1.0f / m_Camera.zoom);
	m_Images[m_SelectedImageIndex].MoveBy(delta);
}

void Canvas::DrawImages()
{
	for (const auto& img : m_Images)
	{
		img.Draw();
	}
}

void Canvas::SelectImageByScreenPos(Vector2 pos)
{
	m_SelectedImageIndex = -1;
	UnselectAllImages();

	Vector2 worldPos = GetScreenToWorld2D(pos, m_Camera);
	for (int i = (int)m_Images.size() - 1; i >= 0; i--)
	{
		if (CheckCollisionPointRec(worldPos, m_Images[i].AsRectangle()))
		{
			m_SelectedImageIndex = i;
			m_Images[i].Focus();
			break;
		}
	}
}

void Canvas::UnselectAllImages()
{
	for (auto& image : m_Images)
		image.Unfocus();
}

CanvasImage::CanvasImage(Vector2 pos, uint8_t* data, int width, int height)
	: m_Texture{}, m_Position{pos}
{
	Image image = LoadImageFromRgba(data, width, height);
	m_Texture = LoadTextureFromImage(image);
	UnloadImage(image);
}

CanvasImage::CanvasImage(CanvasImage&& other) noexcept
{
	m_Position = other.m_Position;
	m_Texture = other.m_Texture;
	other.m_Texture = {};
}

CanvasImage& CanvasImage::operator=(CanvasImage&& other) noexcept
{
	m_Position = other.m_Position;
	m_Texture = other.m_Texture;
	other.m_Texture = {};
	return *this;
}

CanvasImage::~CanvasImage()
{
	UnloadTexture(m_Texture);
}

void Ensure(bool condition)
{
	if (!condition)
	{
		TraceLog(LOG_ERROR, "ENSURE: Assertion filed");
		exit(-1);
	}
}

Rectangle CanvasImage::GetWorldBoundaries() const
{
	return AsRectangle();
}

Image CanvasImage::LoadImageFromRgba(uint8_t* data, int width, int height)
{
	PixelFormat format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	int size = GetPixelDataSize(width, height, format);

	Image image{};
	image.data = RL_MALLOC(size);
	if (image.data == nullptr)
	{
		TraceLog(LOG_ERROR, "IMAGE: Cannot alocate memory for image");
		return image;
	}
	memcpy(image.data, data, size);
	image.width = width;
	image.height = height;
	image.mipmaps = 1;
	image.format = format;

	return image;
}

void CanvasImage::Draw() const
{
	DrawTextureV(m_Texture, m_Position, WHITE);
	if (m_Focued)
		DrawRectangleLinesEx(GetWorldBoundaries(), 2.0f, BLUE);
}

void CanvasImage::MoveBy(Vector2 positionChange)
{
	m_Position = Vector2Add(m_Position, positionChange);
}

Rectangle CanvasImage::AsRectangle() const
{
	return Rectangle{m_Position.x, m_Position.y, (float)m_Texture.width, (float)m_Texture.height};
}

void CanvasImage::AdjustFilterToZoomLevel(float zoom)
{
	if (zoom < 1.0f)
		SetTextureFilter(m_Texture, TEXTURE_FILTER_BILINEAR);
	else
		SetTextureFilter(m_Texture, TEXTURE_FILTER_POINT);
}
