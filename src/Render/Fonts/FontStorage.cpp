#include "Render/Fonts/FontStorage.hpp"
#include "Render/Fonts/compressed_roboto.h"
#include "raylib.h"
#include <Trex/Atlas.hpp>
#include <Trex/TextShaper.hpp>
#include <utility>

std::vector<uint8_t> LoadDefaultFontData()
{
	int robotoDataSize;
	unsigned char* robotoData = ::DecompressData(compressed_roboto_ttf, compressed_roboto_ttf_len, &robotoDataSize);
	std::vector<uint8_t> fontData{robotoData, robotoData + robotoDataSize};
	::MemFree(robotoData);
	return fontData;
}

FontStorage::FontStorage()
{
    auto robotoData = LoadDefaultFontData();
    m_Fonts.emplace_back("Roboto", robotoData);
}

const FontInstance& FontStorage::GetDefaultFont(FontSize fontSize)
{
    return m_Fonts.at(0).GetFont(fontSize);
}

const FontInstance& FontStorage::GetFont(FontId id, FontSize fontSize)
{
    if(not IsFontLoaded(id))
    {
        LOG_ERROR("Font with id {} is not loaded. Using default font instead.", id);
        return GetDefaultFont(fontSize);
    }

    return m_Fonts.at(id).GetFont(fontSize);
}

FontId FontStorage::LoadNewFont(const std::string& fontPath)
{
    FontId id = m_Fonts.size();
    const auto fontName = ::GetFileNameWithoutExt(fontPath.c_str());
    m_Fonts.emplace_back(fontName, fontPath);
    return id;
}

bool FontStorage::IsFontLoaded(FontId id) const
{
    return id < m_Fonts.size();
}

FontCollection::FontCollection(std::string name, const std::string& fontPath)
    : m_Name(std::move(name))
{
    LOG_INFO("Loading font {} from {}", m_Name, fontPath);
    unsigned int dataSize = 0;
    unsigned char* data = ::LoadFileData(fontPath.c_str(), &dataSize);
    LoadFontData(std::span<uint8_t>{data, (size_t)dataSize});
    ::UnloadFileData(data);
}

FontCollection::FontCollection(std::string name, const std::span<uint8_t> fontData)
    : m_Name(std::move(name))
{
    LoadFontData(fontData);
}

const FontInstance& FontCollection::GetFont(FontSize fontSize)
{
    if (not IsFontSizeLoaded(fontSize))
    {
        LoadFontWithSize(fontSize);
    }

	return m_FontInstances.at(fontSize);
}

bool FontCollection::IsFontSizeLoaded(FontSize fontSize) const
{
    return m_FontInstances.find(fontSize) != m_FontInstances.end();
}

void FontCollection::LoadFontData(std::span<uint8_t> fontData)
{
    m_FontData = std::vector<uint8_t>{fontData.begin(), fontData.end()};
}

Image GetAtlasAsTransparentBitmapImage(Trex::Atlas& atlas)
{
    // Rearrange the atlas 1-byte GRAY bitmap into 2-byte GRAY_ALPHA bitmap.
    const auto& bitmap = atlas.GetBitmap();
    int dataSize = bitmap.size() * 2;
    auto* data = (uint8_t*)RL_MALLOC(dataSize);

    for (int i = 0; i < bitmap.size(); ++i)
    {
        data[i * 2] = 0; // Gray, font color
        data[i * 2 + 1] = 255 - bitmap[i]; // Alpha
    }

    Image atlasImage;
    atlasImage.data = data;
    atlasImage.width = atlas.GetWidth(); // width of the atlas bitmap
    atlasImage.height = atlas.GetHeight(); // height of the atlas bitmap
    atlasImage.mipmaps = 1;
    atlasImage.format = PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA;
    return atlasImage;
}

void FontCollection::LoadFontWithSize(FontSize fontSize)
{
    LOG_INFO("Loading font {} with size {}", m_Name, fontSize);

    Trex::Atlas fontAtlas{m_FontData, (int)fontSize, Trex::Charset::Full()};

    // Text shaper
    auto textShaper = std::make_unique<Trex::TextShaper>(fontAtlas);
    // Texture
    Image atlasImage = GetAtlasAsTransparentBitmapImage(fontAtlas);
    auto fontTexture = std::make_unique<Texture2D>(::LoadTextureFromImage(atlasImage)); // TODO: UnloadTexture
    SetTextureFilter(*fontTexture, TEXTURE_FILTER_BILINEAR);
    ::UnloadImage(atlasImage);

    m_FontInstances[fontSize] = { std::move(textShaper), std::move(fontTexture) };
}