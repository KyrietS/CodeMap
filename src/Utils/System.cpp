#include "pch.hpp"
#include "System.hpp"
#include "Utils/Strings.hpp"

namespace Utils::System
{

	std::string GetSystemFontDirPath()
	{
#ifdef _WIN32
		TCHAR windir[MAX_PATH];
		UINT size = GetWindowsDirectory(windir, MAX_PATH);

		if (size == 0)
		{
			LOG_ERROR("Cannot get directory with Windows fonts");
			return "";
		}
		if (size > MAX_PATH)
		{
			LOG_ERROR("Windows font directory path is too long ({} where max is {})", size, MAX_PATH);
			return "";
		}

		return Utils::Strings::ToUtf8(windir) + "\\Fonts";

#elif defined(_APPLE_) && defined(_MACH_)
		return "/Library/Fonts";
#elif defined(linux) || defined(__linux)
		return "/usr/share/fonts"; // also: "/usr/local/share/fonts"
#else
#error Missing implementation for this platform
#endif
	}

}