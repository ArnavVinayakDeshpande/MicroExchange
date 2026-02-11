#include <core/Error.h>
#include <Logger.h>

namespace MicroEx::Error
{
	
	static bool g_IsInitialized = false;

	void MICROEX_API Initialize(Config config) noexcept
	{
		if (g_IsInitialized)
			return;

		if (Log::Core::IsInitialized())
			return;

		Log::Core::Initialize();
		g_IsInitialized = true;
	}

	void MICROEX_API Terminate() noexcept
	{
		g_IsInitialized = false;
	}

	bool MICROEX_API IsInitialized() noexcept
	{
		return g_IsInitialized;
	}

	static void PreAbort()
	{
		// Market Freeze calls when we write market closed
		// Serialize and dumping all information calls when we write that
		// Serializing trades etc.

		MICROEX_LOG_CR_INF("[Aborting the Program...]")
	}

	void MICROEX_API Post(Severity sev, const char* msg) noexcept
	{
		const char* content = msg ? msg : "[Empty Message]";

		switch (sev)
		{
		case Severity::Warn:
			MICROEX_LOG_CR_WRN(content);
			break;

		case Severity::Error:
			MICROEX_LOG_CR_ERR(content);
			break;

		case Severity::Critical:
			MICROEX_LOG_CR_CRT(content);
			break;

		case Severity::Fatal:
			MICROEX_LOG_CR_CRT(content);
			PreAbort();
			std::abort();
			break;

		default:
			Post(Severity::Fatal, "Unknown Error Severity");
			break;
		}
	}

#ifdef MICROEX_DEBUG_BUILD

	void MICROEX_API SoftAssert(const char* msg) noexcept
	{
		PostError(msg);
	}

	[[noreturn]]
	void MICROEX_API HardAssert(const char* msg) noexcept
	{
		PostFatal(msg);
	}

#else

	void MICROEX_API SoftAssert(const char* msg) noexcept
	{
	}

	[[noreturn]]
	void MICROEX_API HardAssert(const char* msg) noexcept
	{
		PostFatal(msg);
	}

#endif

}
