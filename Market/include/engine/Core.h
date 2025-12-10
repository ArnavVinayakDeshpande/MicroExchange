#pragma once

#include <cstdint>
#include <functional>

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

	using UUID = uint64_t;
	using StockID = UUID;
	using OrderID = UUID;
	using TradeID = UUID;
	using Price = double;
	using Quantity = uint32_t;

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

	class Trade;

	using TradeCallbackFunc = std::function<void(Trade&)>;

}