#pragma once

#pragma warning(push, 0)

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#pragma warning(pop)


class Logger
{
public:
	static void Init();

	static spdlog::logger* GetLogger() { return s_Logger.get(); }
private:
	static std::unique_ptr<spdlog::logger> s_Logger;
};

#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Logger::GetLogger(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(Logger::GetLogger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(Logger::GetLogger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(Logger::GetLogger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Logger::GetLogger(), __VA_ARGS__)
