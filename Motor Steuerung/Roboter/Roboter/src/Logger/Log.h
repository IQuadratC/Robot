#pragma once
#include<memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	inline static std::shared_ptr<spdlog::logger>& GetLidarLogger() { return s_LidarLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
	static std::shared_ptr<spdlog::logger> s_LidarLogger;
};

#define LIDAR_LOG_ERROR(...) ::Log::GetLidarLogger()->error(__VA_ARGS__)
#define LIDAR_LOG_WARN(...) ::Log::GetLidarLogger()->warn(__VA_ARGS__)
#define LIDAR_LOG_INFO(...) ::Log::GetLidarLogger()->info(__VA_ARGS__)
#define LIDAR_LOG_TRACE(...) ::Log::GetLidarLogger()->trace(__VA_ARGS__)


#define LOG_ERROR(...) ::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_WARN(...) ::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...) ::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...) ::Log::GetLogger()->trace(__VA_ARGS__)


