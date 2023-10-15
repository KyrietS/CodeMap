#pragma once
#include "pch.hpp"

using FontId = unsigned int;
using FontSize = float;

namespace Trex
{
class TextShaper;
}
class FontCollection;
struct Texture;

struct FontInstance
{
    std::unique_ptr<Trex::TextShaper> Shaper;
    std::unique_ptr<const Texture> Texture;
};

class FontStorage
{
public:
    FontStorage();
    const FontInstance& GetDefaultFont(FontSize);
    const FontInstance& GetFont(FontId, FontSize);
    FontId LoadNewFont(const std::string& fontPath);

private:
    bool IsFontLoaded(FontId) const;
    std::vector<FontCollection> m_Fonts;
};

class FontCollection
{
public:
    FontCollection(std::string name, const std::string& fontPath);
    FontCollection(std::string name, std::span<uint8_t> fontData);
    FontCollection(FontCollection&&)  noexcept = default;
    const std::string& Name() const { return m_Name; }
    const FontInstance& GetFont(FontSize);

private:
    bool IsFontSizeLoaded(FontSize) const;
    void LoadFontData(std::span<uint8_t> fontData);
    void LoadFontWithSize(FontSize);

    std::string m_Name;
    std::vector<uint8_t> m_FontData;

    std::map<FontSize, FontInstance> m_FontInstances;
};