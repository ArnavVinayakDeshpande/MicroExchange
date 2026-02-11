#pragma once

#include <core/Core.h>
#include <core/Price.h>
#include <core/Error.h>

#include <Logger.h>
#include <UUIDGenerator.h>
#include <Timestamp.h>

#include <market/Market.h>

namespace MicroEx
{

	void MICROEX_API Initialize();

	void MICROEX_API Terminate();

	bool MICROEX_API IsInitialized();

}
