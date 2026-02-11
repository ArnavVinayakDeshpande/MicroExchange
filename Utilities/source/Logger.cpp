#include <Logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace MicroEx::Log::Core
{

	static std::shared_ptr<spdlog::logger> g_CoreLogger(nullptr);

	void Initialize()
	{
		if (g_CoreLogger)
			return;

		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::string(GetCoreLogPath()), true);
	
		constexpr const char* pattern = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] [t:%t] %v";

		sink->set_pattern(pattern);

		g_CoreLogger = std::make_shared<spdlog::logger>("CORE", spdlog::sinks_init_list{ sink });
		g_CoreLogger->set_level(spdlog::level::trace);

		spdlog::flush_on(spdlog::level::err);

		spdlog::register_logger(g_CoreLogger);
	}

	void Terminate()
	{
		if (!g_CoreLogger)
			g_CoreLogger.reset();
	}

	std::shared_ptr<spdlog::logger> GetLogger()
	{
		return g_CoreLogger;
	}

	bool IsInitialized()
	{
		return g_CoreLogger.get() != nullptr;
	}

}

namespace MicroEx::Log::Client
{

	static std::shared_ptr<spdlog::logger> g_ClientLogger(nullptr);

	void MICROEX_API Initialize(const std::filesystem::path& filepath)
	{
		if (g_ClientLogger)
			return;

		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath.string(), true);

		constexpr const char* pattern = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] [t:%t] %v";

		consoleSink->set_pattern(pattern);
		fileSink->set_pattern(pattern);

		g_ClientLogger = std::make_shared<spdlog::logger>("CLIENT", spdlog::sinks_init_list{ consoleSink, fileSink });
		g_ClientLogger->set_level(spdlog::level::trace);

		spdlog::flush_on(spdlog::level::err);

		spdlog::register_logger(g_ClientLogger);
	}

	void MICROEX_API Terminate()
	{
		if (!g_ClientLogger)
			g_ClientLogger.reset();
	}

	std::shared_ptr<spdlog::logger> MICROEX_API GetLogger()
	{
		return g_ClientLogger;
	}

	bool MICROEX_API IsInitialized()
	{
		return g_ClientLogger.get() != nullptr;
	}

}
