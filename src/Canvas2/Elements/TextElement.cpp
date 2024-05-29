#include "TextElement.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Utils/Strings.hpp"

namespace Elements
{
	void TextElement::Data::SetUtf8Text(const std::string& utf8Content)
	{
		Text = Utils::Strings::Utf8ToUnicode(utf8Content);
	}

	std::string TextElement::Data::GetTextInUtf8() const
	{
		return Utils::Strings::ToUtf8(Text);
	}

	void TextElement::Draw()
	{
		Renderer::DrawText(m_Data.Text, m_Data.Position, m_Data.FontSize, m_Data.FontId);
	}

	void TextElement::OnEvent(Event& event)
	{
		if (InEditMode)
		{

		}
	}

	void TextElement::MoveBy(float x, float y)
	{
		m_Data.Position += glm::vec2 { x, y };
	}

	Box TextElement::GetBoundingBox() const
	{
		const glm::vec2 padding = { 10.f, 10.f };
		const auto textMeasure = Renderer::MeasureText(m_Data.Text, m_Data.FontSize, m_Data.FontId);
		glm::vec2 corner = m_Data.Position + textMeasure.Offset - padding;
		glm::vec2 size = textMeasure.Size + 2.0f * padding;

		return {
			.x = corner.x,
			.y = corner.y,
			.width = size.x,
			.height = size.y
		};
	}
}