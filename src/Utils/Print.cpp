#include "pch.hpp"
#include "Print.hpp"

std::vector<std::string_view> split(std::string_view text, std::string_view pattern)
{
	std::vector<std::string_view> result;
	std::string_view leftover = text;

	auto pos = leftover.find(pattern);
	while (pos != text.npos)
	{
		auto fragment = leftover.substr(0, pos);
		result.push_back(fragment);
		leftover = leftover.substr(pos + pattern.size(), leftover.size() - pos);
		pos = leftover.find(pattern);
	}

	result.push_back(leftover);

	return result;
}
