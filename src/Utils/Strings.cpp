#include "pch.hpp"
#include "Strings.hpp"

// This piece of crap Visual Studio always defines __cplusplus as 199711L
#ifdef _MSC_VER
#define UTF_CPP_CPLUSPLUS 202002L
#endif

#include "utf8.h"


namespace Utils::Strings
{

	std::string ToUtf8(char32_t character)
	{
		std::u32string str(1, character);
		return ToUtf8(str);
	}

	std::string ToUtf8(const char32_t* rawString)
	{
		std::u32string str = rawString;
		return ToUtf8(str);
	}

	std::string ToUtf8(std::u32string_view view)
	{
		return utf8::utf32to8(view);
	}

	std::string ToUtf8(char16_t character)
	{
		std::u16string str(1, character);
		return ToUtf8(str);
	}

	std::string ToUtf8(const char16_t* rawString)
	{
		std::u16string str = rawString;
		return ToUtf8(str);
	}

	std::string ToUtf8(std::u16string_view view)
	{
		return utf8::utf16to8(view);
	}

	std::string ToUtf8(wchar_t character)
	{
		switch (sizeof(character))
		{
		case 1: return std::string(1, static_cast<char>(character));
		case 2: return ToUtf8(static_cast<char16_t>(character));
		case 4: return ToUtf8(static_cast<char32_t>(character));
		default:
			LOG_ERROR("Unsupported wchar_t size!");
			return std::string();
		}
	}

	std::string ToUtf8(const wchar_t* rawString)
	{
		std::wstring str = rawString;
		std::string result;
		for (const wchar_t c : str)
		{
			result += ToUtf8(c);
		}

		return result;
	}

	std::vector<std::string_view> SplitToLines(const std::string& str, bool keepNewlines)
	{
		std::vector<std::string_view> result;

		auto lineBegin = str.begin();
		while (lineBegin < str.end())
		{
			auto lineEnd = std::find(lineBegin, str.end(), '\n');
			auto lineEndWithNewline = lineEnd != str.end() ? lineEnd + 1 : lineEnd;
			result.emplace_back(lineBegin, keepNewlines ? lineEndWithNewline : lineEnd);
			lineBegin = lineEnd == str.end() ? str.end() : lineEnd + 1;
		}

		if (str.ends_with('\n') && !keepNewlines)
			result.emplace_back("");

		return result;
	}
}