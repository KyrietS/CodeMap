#pragma once

#include "raylib.h"

// TODO: Replace this whole module with spdlog

std::vector<std::string_view> split(std::string_view text, std::string_view pattern);

// TODO: Uncomment when spdlog is in use
//#include <spdlog/fmt/ostr.h>
//
//template<typename OStream>
//inline OStream& operator<<(OStream& os, const Vector2& vector)
//{
//	return os << '(' << vector.x, << ", " << vector.y << ')';
//}

inline void print(const Vector2& v)
{
	std::cout << "(" << v.x << ", " << v.y << ")";
}

template<typename T>
void print(T arg)
{
	std::cout << arg << std::endl;
}

template<typename T>
void print(std::string_view text, T arg)
{
	auto fragments = split(text, "{}");
	assert(fragments.size() == 2);
	std::cout << fragments[0] << arg << fragments[1] << std::endl;
}

template<typename T1, typename T2>
void print(std::string_view text, T1 arg1, T2 arg2)
{
	auto fragments = split(text, "{}");
	assert(fragments.size() == 3);
	std::cout << fragments[0] << arg1 << fragments[1] << arg2 << fragments[2] << std::endl;
}

template<typename T1, typename T2, typename T3>
void print(std::string_view text, T1 arg1, T2 arg2, T3 arg3)
{
	auto fragments = split(text, "{}");
	assert(fragments.size() == 4);
	std::cout << fragments[0] << arg1 << fragments[1] << arg2 << fragments[2] << arg3 << fragments[3] << std::endl;
}

template<typename T1, typename T2, typename T3, typename T4>
void print(std::string_view text, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
	auto fragments = split(text, "{}");
	assert(fragments.size() == 5);
	std::cout << fragments[0] << arg1 << fragments[1] << arg2 << fragments[2]
		<< arg3 << fragments[3] << arg4 << fragments[4] << std::endl;
}
