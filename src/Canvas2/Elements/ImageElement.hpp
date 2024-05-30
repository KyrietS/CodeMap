#pragma once
#include "IElement.hpp"

namespace Elements
{
	class ImageElement : public IElement
	{
	public:
		struct Data
		{
			glm::vec2 Position = { 0.0f, 0.0f };
			int Width = 0;
			int Height = 0;
			std::shared_ptr<unsigned int> TextureId = nullptr;
		};

		void Draw() override;
		void MoveBy(float x, float y) override;
		Box GetBoundingBox() const override;

		Data& GetData() { return m_Data; }
		const Data& GetData() const { return m_Data; }

	private:
		Data m_Data;
	};
}