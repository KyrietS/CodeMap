#pragma once

#include "raylib.h"
#include <cstdint>
#include <vector>


struct CanvasProps
{
	Color BackgroundColor = RAYWHITE;
};

// TODO: Use composition (entity component system) instead of inheritance!
class IFocusable
{
public:
	virtual void Focus() = 0;
	virtual void Unfocus() = 0;
	virtual Rectangle GetWorldBoundaries() const = 0;
};

class CanvasImage : public IFocusable
{
public:
	// Expects RGBA pixel format.
	CanvasImage(Vector2 pos, uint8_t* data, int width, int height);
	CanvasImage(CanvasImage&&) noexcept;
	CanvasImage& operator=(CanvasImage&&) noexcept;
	~CanvasImage();
	void Draw() const;
	void MoveBy(Vector2);
	Rectangle AsRectangle() const;
	void AdjustFilterToZoomLevel(float zoom);

	void Focus() override { m_Focued = true; }
	void Unfocus() override { m_Focued = false; }
	Rectangle GetWorldBoundaries() const override;

private:
	Image LoadImageFromRgba(uint8_t* data, int width, int height);

private:
	Texture2D m_Texture;
	Vector2 m_Position;
	bool m_Focued = false;
};

class Canvas
{
public:
	Canvas();
	void Draw();
	void OnUpdate();

private:	
	void MoveCanvasWithMouse();
	void MoveObjectOnCanvasWithMouse();
	void ZoomCanvas(float zoomChange);
	void HandlePasteImage();
	void SelectImageByScreenPos(Vector2);
	void UnselectAllImages();

	// Drawing
	void DrawGrid();
	void DrawGui();
	void DrawImages();

private:
	CanvasProps m_Props;
	Camera2D m_Camera;
	std::vector<CanvasImage> m_Images;
	int m_SelectedImageIndex = -1;
};
