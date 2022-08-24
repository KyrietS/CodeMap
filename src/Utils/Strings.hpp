#pragma once
#include <string>

namespace Utils::Strings
{
	std::string ToUtf8(char32_t);
	std::string ToUtf8(const char32_t*);
	std::string ToUtf8(std::u32string_view);

	std::string ToUtf8(char16_t);
	std::string ToUtf8(const char16_t*);
	std::string ToUtf8(std::u16string_view);

	std::string ToUtf8(wchar_t);
	std::string ToUtf8(const wchar_t*);
}