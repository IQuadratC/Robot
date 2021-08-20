#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	inline static std::shared_ptr<spdlog::logger>& GetLidarLogger() { return s_LidarLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetRoboterLogger() { return s_RoboterLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetNetworkLogger() { return s_NetworkLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
	static std::shared_ptr<spdlog::logger> s_LidarLogger;
	static std::shared_ptr<spdlog::logger> s_RoboterLogger;
	static std::shared_ptr<spdlog::logger> s_NetworkLogger;
};