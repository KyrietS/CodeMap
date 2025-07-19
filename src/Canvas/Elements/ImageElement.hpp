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

		ImageElement() : m_Data(GetDefaultData()) {}

		void Draw() override;
		void MoveBy(float x, float y) override;
		Box GetBoundingBox() const override;
		void Accept(IElementVisitor& visitor) override;

		Data& GetData() { return m_Data; }
		const Data& GetData() const { return m_Data; }

		static Data& GetDefaultData() {
			static Data defaultData;
			return defaultData;
		}

	private:
		Data m_Data;
	};
}