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

	using CanvasControllers = std::vector<std::unique_ptr<IController>>;
}