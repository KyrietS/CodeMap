#include "pch.hpp"
#include "Canvas/Controllers/CommonKeyboardShortcutsController.hpp"
#include "Input.hpp"
#include "Events/CanvasEvents.hpp"

void CommonKeyboardShortcutsController::OnUpdate()
{
	const bool ignoreTextMode = true;

	// Ctrl + Z
	if (Input::IsKeyDown(Key::LeftControl, ignoreTextMode) && Input::IsKeyPressed(Key::Z, ignoreTextMode))
	{
		LOG_DEBUG("Ctrl + Z");
		m_EventQueue.Push(Events::Canvas::Undo{});
	}

	// Ctrl + Y
	if (Input::IsKeyDown(Key::LeftControl, ignoreTextMode) && Input::IsKeyPressed(Key::Y, ignoreTextMode))
	{
		LOG_DEBUG("Ctrl + Y");
		m_EventQueue.Push(Events::Canvas::Redo{});
	}

	// Ctrl + X
	if (Input::IsKeyDown(Key::LeftControl, ignoreTextMode) && Input::IsKeyPressed(Key::X, ignoreTextMode))
	{
		LOG_DEBUG("Ctrl + X");
		m_EventQueue.Push(Events::Canvas::Cut{});
	}

	// Ctrl + C
	if (Input::IsKeyDown(Key::LeftControl, ignoreTextMode) && Input::IsKeyPressed(Key::C, ignoreTextMode))
	{
		LOG_DEBUG("Ctrl + C");
		m_EventQueue.Push(Events::Canvas::Copy{});
	}

	// Ctrl + V
	if (Input::IsKeyDown(Key::LeftControl, ignoreTextMode) && Input::IsKeyPressed(Key::V, ignoreTextMode))
	{
		LOG_DEBUG("Ctrl + V");
		m_EventQueue.Push(Events::Canvas::Paste{});
	}
}