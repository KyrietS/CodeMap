#pragma once
#include <string>
#include <functional>
#include "Events/Event.hpp"


class Window
{
public:
	static void Init(uint32_t width, uint32_t height, const std::string& title);
	static uint32_t GetWidth();
	static uint32_t GetHeight();

	static void PollEvents();
	static void PollEventsOrWait();
	static void SetEventCallback(std::function<void(Event&)>);

	static void Close();
	static bool ShouldClose(); // Close icon clicked

	static std::function<void(Event&)> s_EventCallback;
};

