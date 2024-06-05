#pragma once
#include <string>
#include <functional>
#include "Events/Event.hpp"
#include "Events/EventQueue.hpp"
#include "Cursor.hpp"


class Window
{
public:
	static void Init(uint32_t width, uint32_t height, const std::string& title, EventQueue*);
	static uint32_t GetWidth();
	static uint32_t GetHeight();
	static Cursor& GetMouseCursor();

	static void SetWindowTitle(const std::string& title);

	static void PollEvents();
	static void PollEventsOrWait();
	static void Close();

	static bool ShouldClose(); // Close icon clicked

private:
	template<typename T>
	static void EventCallback(T& event)
	{
		s_EventQueue->Push(event);
	}

	static EventQueue* s_EventQueue;
	static std::unique_ptr<Cursor> s_MouseCursor;
};

