#pragma once

#pragma warning(push, 0)

// Hack to prevent linker conflict between raylib and Windows.h
#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#if defined(_WIN32)
	#undef near
	#undef far
#endif

#pragma warning(pop)


class Logger
{
public:
	static void Init();

	static spdlog::logger* GetLogger() { return s_Logger.get(); }
private:
	static std::unique_ptr<spdlog::logger> s_Logger;
};


template<typename OStream, typename Vector2Type>
inline OStream& operator<<(OStream& os, const Vector2Type& vector)
{
	return os << '(' << vector.x << ", " << vector.y << ')';
}


#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Logger::GetLogger(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(Logger::GetLogger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(Logger::GetLogger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(Logger::GetLogger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Logger::GetLogger(), __VA_ARGS__)
