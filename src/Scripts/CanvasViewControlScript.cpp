#include "pch.hpp"
#include "CanvasViewControlScript.hpp"


void CanvasViewControlScript::OnUpdate()
{
	float zoomChange = GetMouseWheelMove();
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

		for (auto [entity, texture] : Canvas::Get().GetComponents<Components::Sprite>().each())
		{
			AdjustFilterToZoomLevel(m_Camera.zoom, texture);
		}
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		// Place for some debugging code...
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
	{
		MoveCanvasWithMouse();
	}
}

void CanvasViewControlScript::AdjustFilterToZoomLevel(float zoom, Texture2D& texture)
{
	std::cout << "zoom = " << zoom << std::endl;
	if (zoom < 1.0f)
		SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
	else
		SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}

void CanvasViewControlScript::MoveCanvasWithMouse()
{
	Vector2 delta = GetMouseDelta();
	delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);
	m_Camera.target = Vector2Add(m_Camera.target, delta);
}
