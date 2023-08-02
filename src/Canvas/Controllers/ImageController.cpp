#include "pch.hpp"
#include "ImageController.hpp"
#include "Input.hpp"
#include "clip.h"
#include "Prefabs/ImageEntity.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"

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
			uint8_t r = clipboardImage.data()[pixelOffset + imageSpec.red_shift / 8];
			uint8_t g = clipboardImage.data()[pixelOffset + imageSpec.green_shift / 8];
			uint8_t b = clipboardImage.data()[pixelOffset + imageSpec.blue_shift / 8];
			uint8_t a = bytesPerPixel == 4 ? clipboardImage.data()[pixelOffset + imageSpec.alpha_shift / 8] : 255;
			result.push_back(r);
			result.push_back(g);
			result.push_back(b);
			result.push_back(a);
		}
	}
	return result;
}
} // namespace

void ImageController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Canvas::Paste>(BIND_EVENT(ImageController::OnPasteEvent));
}

void ImageController::OnPasteEvent(const Events::Canvas::Paste& event)
{
	if (clip::has(clip::image_format()))
	{
		PasteImageFromClipboard();
		m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
	}
}

void ImageController::PasteImageFromClipboard()
{
	clip::image clipboardImage;
	if (!clip::get_image(clipboardImage) || !clipboardImage.is_valid())
	{
		return LOG_WARN("CLIPBOARD: Failed to paste an image from clipboard");
	}

	clip::image_spec imageSpec = clipboardImage.spec();
	std::vector<uint8_t> rgbaData = PrepareRgbaData(clipboardImage);
	glm::vec2 imagePos = Input::GetWorldMousePosition();

	auto entity = ImageEntity(
		Canvas::Get().CreateEntity(), m_EventQueue).Build(imagePos, rgbaData.data(), imageSpec.width, imageSpec.height);

	m_EventQueue.Push(Events::Canvas::SetFocus{ entity });
}