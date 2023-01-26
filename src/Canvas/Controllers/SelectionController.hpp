#pragma once

#include "Canvas/Components.hpp"
#include "IController.hpp"


class SelectionController : public IController
{
public:
	void OnUpdate() override;
};