#pragma once

#include "Canvas/Components.hpp"
#include "IController.hpp"
#include "Events/CanvasEvents.hpp"

class SelectionController : public IController
{
public:
	void OnUpdate() override;
	void OnEvent(Event& event) override;

private:
	void OnSetFocus(const Events::Canvas::SetFocus&);
};