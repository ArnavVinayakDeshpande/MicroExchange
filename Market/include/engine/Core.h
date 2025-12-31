#pragma once

#include <cstdint>
#include <functional>
#include <fstream>

#if defined(MICROEX_BUILD_SHARED)
	#if defined(MICROEX_SOURCE)
		#define MICROEX_API __declspec(dllexport)
	#else
		#define MICROEX_API __declspec(dllimport)
	#endif
#else
	#define MICROEX_API
#endif

namespace MicroEx
{

	using uuid_t = uint64_t;
	using company_id_t = uuid_t;
	using order_id_t = uuid_t;
	using trade_it_t = uuid_t;
	using price_t = double;
	using quantity_t = uint32_t;

	enum class OrderSide : uint8_t
	{
		Buyer,
		Seller
	};

	enum class OrderType : uint16_t
	{
		Market,
		Limit
	};

	enum class OrderStatus : uint16_t
	{
		None,
		Filled,
		PartiallyFilled,
		NoFilled
	};

	class Trade;

	using TradeCallbackFunc = std::function<void(Trade&)>;

}