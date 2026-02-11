#include <MicroEx/MicroEx.h>

int32_t main(int32_t argc, const char* argv[])
{
	MicroEx::Initialize();

	MicroEx::Log::Client::Initialize("MicroExClient.log");

	MICROEX_LOG_CL_TRC("This is a trace log from client");
	MICROEX_LOG_CL_CRT("This is a critical log from client");

	

	MicroEx::Terminate();

	return 0;
}