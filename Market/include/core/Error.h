#pragma once

#include <core/Core.h>
#include <filesystem>
#include <spdlog/fmt/fmt.h>

#define MICROEX_ERR_WRN(...) ::MicroEx::Error::PostWarn(fmt::format(__VA_ARGS__).c_str());
#define MICROEX_ERR_ERR(...) ::MicroEx::Error::PostError(fmt::format(__VA_ARGS__).c_str());
#define MICROEX_ERR_CRT(...) ::MicroEx::Error::PostCritical(fmt::format(__VA_ARGS__).c_str());
#define MICROEX_ERR_FTL(...) ::MicroEx::Error::PostFatal(fmt::format(__VA_ARGS__).c_str());

namespace MicroEx::Error
{

	enum class Severity : std::uint8_t
	{
		Warn,
		Error,
		Critical,
		Fatal
	};

	struct MICROEX_API Config final
	{
		std::filesystem::path LogPath;

		Config() noexcept
			:
			LogPath(DefaultLogPath)
		{
		}

		explicit Config(const std::filesystem::path& logPath) noexcept
			:
			LogPath(logPath)
		{
		}

		~Config() noexcept = default;

	public:
		static constexpr const char* DefaultLogPath = "MicroEx.log";
	};

	void MICROEX_API Initialize(Config config) noexcept;

	void MICROEX_API Terminate() noexcept;

	void MICROEX_API Post(Severity sev, const char* msg) noexcept;

	bool MICROEX_API IsInitialized() noexcept;

	inline void MICROEX_API PostWarn(const char* msg) noexcept
	{
		::MicroEx::Error::Post(Severity::Warn, msg);
	}

	inline void MICROEX_API PostError(const char* msg) noexcept
	{
		::MicroEx::Error::Post(Severity::Error, msg);
	}

	inline void MICROEX_API PostCritical(const char* msg) noexcept
	{
		::MicroEx::Error::Post(Severity::Critical, msg);
	}

	[[noreturn]]
	inline void MICROEX_API PostFatal(const char* msg) noexcept
	{
		::MicroEx::Error::Post(Severity::Fatal, msg);
	}

	void MICROEX_API SoftAssert(const char* msg) noexcept;

	[[noreturn]]
	void MICROEX_API HardAssert(const char* msg) noexcept;

}
