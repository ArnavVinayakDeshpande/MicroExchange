#pragma once

#include <core/Core.h>
#include <spdlog/logger.h>
#include <memory>
#include <filesystem>

#ifdef MICROEX_ENABLE_LOGGING
	#define MICROEX_LOG_CORE_LOGGER ::MicroEx::Log::Core::GetLogger()
	#define MICROEX_LOG_CLIENT_LOGGER :: MicroEx::Log::Client::GetLogger()

	#ifdef MICROEX_SOURCE
		#define MICROEX_LOG_CR_TRC(...) MICROEX_LOG_CORE_LOGGER->trace(__VA_ARGS__);
		#define MICROEX_LOG_CR_INF(...) MICROEX_LOG_CORE_LOGGER->info(__VA_ARGS__);
		#define MICROEX_LOG_CR_WRN(...) MICROEX_LOG_CORE_LOGGER->warn(__VA_ARGS__);
		#define MICROEX_LOG_CR_ERR(...) MICROEX_LOG_CORE_LOGGER->error(__VA_ARGS__);
		#define MICROEX_LOG_CR_CRT(...) MICROEX_LOG_CORE_LOGGER->critical(__VA_ARGS__);

		#ifdef MICROEX_DEBUG_BUILD
			#define MICROEX_LOG_CR_DBG(...) MICROEX_LOG_CORE_LOGGER->debug(__VA_ARGS__);
		#else
			#define MICROEX_LOG_CR_DBG(...)
		#endif
	#else
		#define MICROEX_LOG_CR_TRC(...) 
		#define MICROEX_LOG_CR_INF(...) 
		#define MICROEX_LOG_CR_WRN(...) 
		#define MICROEX_LOG_CR_ERR(...) 
		#define MICROEX_LOG_CR_CRT(...) 
		#define MICROEX_LOG_CR_DBG(...)
	#endif

	#define MICROEX_LOG_CL_TRC(...) MICROEX_LOG_CLIENT_LOGGER->trace(__VA_ARGS__);
	#define MICROEX_LOG_CL_INF(...) MICROEX_LOG_CLIENT_LOGGER->info(__VA_ARGS__);
	#define MICROEX_LOG_CL_WRN(...) MICROEX_LOG_CLIENT_LOGGER->warn(__VA_ARGS__);
	#define MICROEX_LOG_CL_ERR(...) MICROEX_LOG_CLIENT_LOGGER->error(__VA_ARGS__);
	#define MICROEX_LOG_CL_CRT(...) MICROEX_LOG_CLIENT_LOGGER->critical(__VA_ARGS__);

	#ifdef MICROEX_DEBUG_BUILD
		#define MICROEX_LOG_CL_DBG(...) MICROEX_LOG_CLIENT_LOGGER->debug(__VA_ARGS__);
	#else
		#define MICROEX_LOG_CL_DBG(...)
	#endif

	#ifdef MICROEX_LOGGING_AUTO
		#ifdef MICROEX_SOURCE
			#define MICROEX_LOG_TRC(...) MICROEX_LOG_CR_TRC(__VA_ARGS__);
			#define MICROEX_LOG_INF(...) MICROEX_LOG_CR_INF(__VA_ARGS__);
			#define MICROEX_LOG_WRN(...) MICROEX_LOG_CR_WRN(__VA_ARGS__);
			#define MICROEX_LOG_ERR(...) MICROEX_LOG_CR_ERR(__VA_ARGS__);
			#define MICROEX_LOG_CRT(...) MICROEX_LOG_CR_CRT(__VA_ARGS__);

			#ifdef MICROEX_DEBUG_BUILD
				#define MICROEX_LOG_DBG(...) MICROEX_LOG_CR_DBG(__VA_ARGS__);
			#else
				#define MICROEX_LOG_DBG(...)
			#endif
		#endif
			#define MICROEX_LOG_TRC(...) MICROEX_LOG_CL_TRC(__VA_ARGS__);
			#define MICROEX_LOG_INF(...) MICROEX_LOG_CL_INF(__VA_ARGS__);
			#define MICROEX_LOG_WRN(...) MICROEX_LOG_CL_WRN(__VA_ARGS__);
			#define MICROEX_LOG_ERR(...) MICROEX_LOG_CL_ERR(__VA_ARGS__);
			#define MICROEX_LOG_CRT(...) MICROEX_LOG_CL_CRT(__VA_ARGS__);

			#ifdef MICROEX_DEBUG_BUILD
				#define MICROEX_LOG_DBG(...) MICROEX_LOG_CL_DBG(__VA_ARGS__);
			#else
				#define MICROEX_LOG_DBG(...)
			#endif
	#else
		#define MICROEX_LOG_TRC(...)
		#define MICROEX_LOG_INF(...)
		#define MICROEX_LOG_WRN(...)
		#define MICROEX_LOG_ERR(...)
		#define MICROEX_LOG_CRT(...)
		#define MICROEX_LOG_DBG(...)
	#endif
#else
	#define MICROEX_LOG_CR_TRC(...)
	#define MICROEX_LOG_CR_INF(...)
	#define MICROEX_LOG_CR_WRN(...)
	#define MICROEX_LOG_CR_ERR(...)
	#define MICROEX_LOG_CR_CRT(...)
	#define MICROEX_LOG_CR_DBG(...)

	#define MICROEX_LOG_CL_TRC(...)
	#define MICROEX_LOG_CL_INF(...)
	#define MICROEX_LOG_CL_WRN(...)
	#define MICROEX_LOG_CL_ERR(...)
	#define MICROEX_LOG_CL_CRT(...)
	#define	MICROEX_LOG_CL_DBG(...)

	#define MICROEX_LOG_TRC(...)
	#define MICROEX_LOG_INF(...)
	#define MICROEX_LOG_WRN(...)
	#define MICROEX_LOG_ERR(...)
	#define MICROEX_LOG_CRT(...)
	#define MICROEX_LOG_DBG(...)
#endif

namespace MicroEx::Log::Core
{
	
	void Initialize();
	void Terminate();
	[[nodiscard]]
	std::shared_ptr<spdlog::logger> GetLogger();
	bool IsInitialized();

}

namespace MicroEx::Log::Client
{
	void MICROEX_API Initialize(const std::filesystem::path& filepath);
	void MICROEX_API Terminate();
	[[nodiscard]]
	std::shared_ptr<spdlog::logger> MICROEX_API GetLogger();
	bool MICROEX_API IsInitialized();
}
