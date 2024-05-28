#include "pch.hpp"
#include "PasteImageController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas2/Elements/ImageElement.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"
#include <clip.h>

namespace
{
	std::vector<uint8_t> PrepareRgbaData(const clip::image& clipboardImage)
	{
		const auto& imageSpec = clipboardImage.spec();
		std::vector<uint8_t> result;
		result.reserve(imageSpec.width * imageSpec.height * 4);

		for (int row = 0; row < imageSpec.height; row++)
		{
			unsigned int bytesPerPixel = imageSpec.bits_per_pixel / 8;
			unsigned int rowOffset = row * imageSpec.bytes_per_row;
			for (int col = 0; col < imageSpec.width; col++)
			{
				unsigned int pixelOffset = rowOffset + col * bytesPerPixel;
				uint8_t r = clipboardImage.data()[ pixelOffset + imageSpec.red_shift / 8 ];
				uint8_t g = clipboardImage.data()[ pixelOffset + imageSpec.green_shift / 8 ];
				uint8_t b = clipboardImage.data()[ pixelOffset + imageSpec.blue_shift / 8 ];
				uint8_t a = bytesPerPixel == 4 ? clipboardImage.data()[ pixelOffset + imageSpec.alpha_shift / 8 ] : 255;
				result.push_back(r);
				result.push_back(g);
				result.push_back(b);
				result.push_back(a);
			}
		}
		return result;
	}

	std::unique_ptr<Elements::ImageElement> CreateImageElement(
		glm::vec2 pos, int width, int height, const std::vector<uint8_t>& rgbaData)
	{
		// TODO: Consider loading the texture in Elements::ImageElement constructor
		auto imageElement = std::make_unique<Elements::ImageElement>();
		auto textureId = Renderer::LoadTextureFromBytes(rgbaData, width, height);
		imageElement->GetData() = {
			.Position = pos,
			.Width = width,
			.Height = height,
			.TextureId = textureId
		};

		return imageElement;
	}

} // namespace

namespace Controllers
{
	void PasteImageController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(OnKeyPressed));
	}

	bool PasteImageController::OnKeyPressed(const Events::Input::KeyPressed& event)
	{
		const bool ctrlVPressed = Input::IsKeyDown(Key::LeftControl) and event.GetKey() == Key::V and not event.IsRepeated();
		if (ctrlVPressed and HasImageInClipboard())
		{
			AddImageFromClipboardToCanvas();
			return true;
		}

		return false;
	}

	bool PasteImageController::HasImageInClipboard()
	{
		return clip::has(clip::image_format());
	}

	void PasteImageController::AddImageFromClipboardToCanvas()
	{
		clip::image clipboardImage;
		if (!clip::get_image(clipboardImage) || !clipboardImage.is_valid())
		{
			LOG_WARN("CLIPBOARD: Failed to paste an image from clipboard");
			return;
		}

		clip::image_spec imageSpec = clipboardImage.spec();
		std::vector<uint8_t> rgbaData = PrepareRgbaData(clipboardImage);
		glm::vec2 imagePos = Input::GetWorldMousePosition(m_Camera);

		auto width = imageSpec.width;
		auto height = imageSpec.height;
		assert(rgbaData.size() == width * height * 4);

		auto imageElement = CreateImageElement(imagePos, width, height, rgbaData);

		auto elementId = m_Elements.Add(std::move(imageElement));

		//m_EventQueue.Push(Events::Canvas::SetFocus { elementId });
	}
}