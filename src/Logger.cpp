#include "pch.hpp"
#include "Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>


std::unique_ptr<spdlog::logger> Logger::s_Logger;


void Logger::Init()
{
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
	consoleSink->set_pattern("%^[%T][%l] %s:%#  %v%$");
	consoleSink->set_level(spdlog::level::debug);

	std::vector<spdlog::sink_ptr> logSinks{ consoleSink };

	s_Logger = std::make_unique<spdlog::logger>("APP", begin(logSinks), end(logSinks));
	s_Logger->set_level(spdlog::level::trace);
	s_Logger->flush_on(spdlog::level::trace);
}
