#include "Log.h"


std::shared_ptr<spdlog::logger> Log::s_Logger;
std::shared_ptr<spdlog::logger> Log::s_LidarLogger;
std::shared_ptr<spdlog::logger> Log::s_RoboterLogger;

void Log::Init()
{
	spdlog::set_pattern("%^[%H:%M:%S] [%n] %v%$");
	spdlog::set_level(spdlog::level::trace);
	s_Logger = spdlog::stdout_color_mt("Main");
	s_LidarLogger = spdlog::stdout_color_mt("Lidar");
	s_RoboterLogger = spdlog::stdout_color_mt("Roboter");
}

