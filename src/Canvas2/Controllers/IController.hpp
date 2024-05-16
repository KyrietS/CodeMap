#pragma once

class Event;

namespace Controllers
{
	class IController
	{
	public:
		virtual ~IController() = default;

		virtual void Draw() {};
		virtual void OnEvent(Event&) {};
	};
}