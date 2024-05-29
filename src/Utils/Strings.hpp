#pragma once
#include <string>

namespace Utils::Strings
{
	std::string ToUtf8(char32_t);
	std::string ToUtf8(const char32_t*);
	std::string ToUtf8(std::u32string_view);
	std::string ToUtf8(std::span<const uint32_t>);

	std::string ToUtf8(char16_t);
	std::string ToUtf8(const char16_t*);
	std::string ToUtf8(std::u16string_view);

	std::string ToUtf8(wchar_t);
	std::string ToUtf8(const wchar_t*);

	std::vector<uint32_t> Utf8ToUnicode(std::span<const char>);

	// To prevent some retards (like me) from calling SplitToLines function
	// with an argument that is temporal value.
	template<typename T>
	void SplitToLines(T&&, bool keepNewlines) = delete;

	// !!! WARNING !!!
	// These functions return vector of spans that point to the original string.
	// If you modify the original string, the spans will be invalidated.
	// The original string must be kept alive for the lifetime of the spans.
	std::vector<std::span<char>> SplitToLines(std::string&, bool keepNewlines = false);
	std::vector<std::span<const char>> SplitToLines(const std::string&, bool keepNewlines = false);
	std::vector<std::span<const uint32_t>> SplitToLines(std::span<const uint32_t>, bool keepNewlines = false);
	std::vector<std::span<uint32_t>> SplitToLines(std::span<uint32_t>, bool keepNewlines = false);
}