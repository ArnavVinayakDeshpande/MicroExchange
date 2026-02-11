#include <MicroEx/MicroEx.h>

namespace MicroEx
{
	
	static bool g_IsInitialized = false;

	void MICROEX_API Initialize()
	{
		if (g_IsInitialized)
			return;

		MicroEx::Log::Core::Initialize();

		MicroEx::Error::Initialize(MicroEx::Error::Config());

		// TODO Add Initialization Config
		g_IsInitialized = true;
	}

	void MICROEX_API Terminate()
	{
		if (!g_IsInitialized)
			return;

		MicroEx::Error::Terminate();
		MicroEx::Log::Core::Terminate();

		if (MicroEx::Log::Client::IsInitialized())
			MicroEx::Log::Client::Terminate();

		g_IsInitialized = false;
	}

	bool MICROEX_API IsInitialized()
	{
		return g_IsInitialized;
	}

}