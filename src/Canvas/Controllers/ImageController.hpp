#pragma once
#include "IController.hpp"

class ImageController : public IController
{
public:
	void OnUpdate() override;

private:
	void PasteImageFromClipboard();
};

