#include <market/Market.h>
#include <Logger.h>

int32_t main(int32_t argc, const char* argv[])
{

	MicroEx::Log::Initiate("Microex.log");

	//for (;;)
	{
	MICROEX_LOG_TRC("Trace");
	MICROEX_LOG_INF("Information {0}, {1}", 120, 12);

	}

	
	MicroEx::Log::Terminate();

	return 0;
}