#include <Logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace MicroEx::Log
{

	static std::shared_ptr<spdlog::logger> g_CoreLogger(nullptr);
	static std::shared_ptr<spdlog::logger> g_ClientLogger(nullptr);

	void MICROEX_API Initiate(const std::filesystem::path& filepath)
	{
		if (g_CoreLogger && g_ClientLogger)
			return;

		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath, true);
	
		constexpr const char* pattern = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] [t:%t] %v";

		consoleSink->set_pattern(pattern);
		fileSink->set_pattern(pattern);

		g_CoreLogger = std::make_shared<spdlog::logger>("CORE", spdlog::sinks_init_list{ fileSink });
		g_ClientLogger = std::make_shared<spdlog::logger>("CLIENT", spdlog::sinks_init_list{ consoleSink, fileSink });
	
		g_CoreLogger->set_level(spdlog::level::trace);
		g_ClientLogger->set_level(spdlog::level::trace);

		spdlog::flush_on(spdlog::level::err);
	
		spdlog::register_logger(g_CoreLogger);
		spdlog::register_logger(g_ClientLogger);
	}

	void MICROEX_API Terminate()
	{
		g_CoreLogger.reset();

		g_ClientLogger.reset();
	}

	std::shared_ptr<spdlog::logger> MICROEX_API GetCoreLogger()
	{
		return g_CoreLogger;
	}

	std::shared_ptr<spdlog::logger> MICROEX_API GetClientLogger()
	{
		return g_ClientLogger;
	}

}