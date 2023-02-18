#include "pch.hpp"
#include "ImageController.hpp"
#include "Input.hpp"
#include "clip.h"
#include "Prefabs/ImageEntity.hpp"

namespace
{
void TransformFromBgraToRgba(uint8_t* data, int size)
{
	for (int i = 0; i < size / 4; i++)
	{
		// Swap first and third byte in pixel.
		std::swap(data[i * 4 + 0], data[i * 4 + 2]);
	}
}
}

void ImageController::OnUpdate()
{
	if (Input::IsKeyDown(Key::LeftControl) && Input::IsKeyPressed(Key::V))
	{
		PasteImageFromClipboard();
	}
}

void ImageController::PasteImageFromClipboard()
{
	clip::image clipboardImage;
	if (!clip::get_image(clipboardImage))
	{
		return LOG_WARN("CLIPBOARD: Failed to paste an image from clipboard");
	}

	clip::image_spec imageSpec = clipboardImage.spec();
	int size = (int)imageSpec.required_data_size();
	uint8_t* data = reinterpret_cast<uint8_t*>(clipboardImage.data());
	TransformFromBgraToRgba(data, size);
	glm::vec2 imagePos = Input::GetWorldMousePosition();

	ImageEntity(Canvas::Get().CreateEntity()).Build(imagePos, data, imageSpec.width, imageSpec.height);
}