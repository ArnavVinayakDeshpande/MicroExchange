#pragma once

#include <engine/Core.h>
#include <spdlog/logger.h>
#include <memory>
#include <filesystem>

#ifdef MICROEX_ENABLE_LOGGING

#ifdef MICROEX_SOURCE
#define MICROEX_GET_LOGGER ::MicroEx::Log::GetCoreLogger()
#else
#define MICROEX_GET_LOGGER ::MicroEx::Log::GetClientLogger()
#endif

#define MICROEX_LOG_TRC(...) MICROEX_GET_LOGGER->trace(__VA_ARGS__);
#define MICROEX_LOG_INF(...) MICROEX_GET_LOGGER->info(__VA_ARGS__);
#define MICROEX_LOG_WRN(...) MICROEX_GET_LOGGER->warn(__VA_ARGS__);
#define MICROEX_LOG_ERR(...) MICROEX_GET_LOGGER->err(__VA_ARGS__);
#define MICROEX_LOG_CRT(...) MICROEX_GET_LOGGER->critical(__VA_ARGS__);

#ifdef MICROEX_DEBUG_BUILD
#define MICROEX_LOG_DBG(...) MICROEX_GET_LOGGER->debug(__VA_ARGS__);
#else
#define MICROEX_LOG_DBG(...)
#endif

#else

#define MICROEX_GET_LOGGER

#define MICROEX_LOG_TRC(...) 
#define MICROEX_LOG_DBG(...) 
#define MICROEX_LOG_INF(...) 
#define MICROEX_LOG_WRN(...) 
#define MICROEX_LOG_ERR(...) 
#define MICROEX_LOG_CRT(...) 

#endif

namespace MicroEx::Log
{

	void MICROEX_API Initiate(const std::filesystem::path& filepath);

	void MICROEX_API Terminate();

	std::shared_ptr<spdlog::logger> MICROEX_API GetCoreLogger();

	std::shared_ptr<spdlog::logger> MICROEX_API GetClientLogger();

}