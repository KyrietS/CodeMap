#pragma once

#include "GuiElement.hpp"

using TextureId = unsigned int;

namespace Gui
{

class Toolbar : public GuiElement
{
public:
	Toolbar();
	void OnUpdate() override;

private:
	enum class Tool
	{
		None,
		Hand,
		Select,
		Arrow,
		Highlight
	};

	void ShowToolButton(Tool tool);

	std::shared_ptr<TextureId> m_IconPlaceholder;
	Tool m_SelectedTool = Tool::None;
};
}