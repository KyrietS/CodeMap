#pragma once

#ifdef _WIN32
	#define NOMINMAX
#endif

#include <iostream>
#include <filesystem>

#include <array>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <functional>

#include <algorithm>
#include <numeric>
#include <chrono>
#include <ranges>
#include <memory>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <locale>

#ifdef _WIN32
	#include <Windows.h>
#endif

#include "Logger.hpp"