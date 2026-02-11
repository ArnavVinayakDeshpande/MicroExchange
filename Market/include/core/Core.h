/**
 * @file Market/include/core/Core.h
 * @author Arnav Deshpande
 * 
 * This file contains macros, typedefs, enums etc. that is shared and required in every file in the project.
*/

#pragma once

#include <cstdint>
#include <functional>
#include <fstream>

/**
 * @def MICROEX_BUILD_SHARED
 * @brief Macro is defined if the project is build as a shared (dynamic) library.
 */
#ifdef MICROEX_BUILD_SHARED
	/**
	 * @def MICROEX_SOURCE
	 * @brief Macro is defined in all source files of the project, external files written by other projects must have this macro undefined.
	 */
	#ifdef MICROEX_SOURCE
		/**
		 * @def MICROEX_API 
		 * @brief Macro is defined if project is build as a shared library, automatically sorts imports and exports for shared libraries.
		 * 
		 * Possible Values:
		 * 		__declspec(dllexport) : If defined in project source files.
		 * 		__declspec(dllimport) : If defined in any other file than project source files.
		 */
		#define MICROEX_API __declspec(dllexport)
	#else
		#define MICROEX_API __declspec(dllimport)
	#endif
#else
	#define MICROEX_API
#endif

#ifdef _WIN32
	#define MICROEX_PLATFORM_WINDOWS
#elif defined(__linux__)
	#define MICROEX_PLATFORM_LINUX
#elif defined(__APPLE__)
	#define MICROEX_PLATFORM_MACOS
#else
	#error UNSUPPORTED PLATFORM
#endif

namespace MicroEx
{

	/**
	 * @typedef uuid_t
	 * Typedefs std::uint64_t. Representation of a numerical, randomly generated Universal Unique IDentifier in the project.
	 */
	using uuid_t = std::uint64_t;

	/**
	 * @typedef company_id_t
	 * Typedefs std::uint64_t. Representation of a numerical, deterministic identifier used for identifying companies whose securities are offered 
	 * in the market.
	 */
	using company_id_t = std::uint64_t;

	/**
	 * @typedef order_id_t
	 * Typedefs MicroEx::uuid_t. UUID for tracking orders placed in the market.
	 */
	using order_id_t = uuid_t;

	/**
	 * @typedef trade_id_t
	 * Typedefs MicroEx::uuid_t. UUID for tracking trades that have been executed in the market.
	 */
	using trade_id_t = uuid_t;

	/**
	 * @typedef quantity_t
	 * Typedefs std::uint32_t. Represents a Quantity of any item/security/commodity in the project.
	 */
	using quantity_t = std::uint32_t;

	/**
	 * @enum OrderSide
	 * @brief Shows if the order is a buy order or a sell order.
	 * 
	 * Possible Values:
	 * 		Buyer
	 * 		Seller
	 * 
	 * Enum values are stored as std::uint8_t.
	 */
	enum class OrderSide : std::uint8_t
	{
		Buyer,
		Seller
	};

	/**
	 * @enum OrderType
	 * @brief Shows the type of order.
	 * 
	 * Possible Values:
	 * 		Market
	 * 		Limit
	 * 
	 * Enum values are stored as std::uint16_t.
	 */
	enum class OrderType : std::uint16_t
	{
		Market,
		Limit
	};

	/**
	 * @enum OrderStatus
	 * @brief Shows the status of order.
	 * 
	 * Possible Values:
	 * 		Unknown
	 * 		Filled
	 * 		PartiallyFilled
	 * 		NotFilled
	 * 
	 * Enum values are stored as std::uint16_t
	 */
	enum class OrderStatus : std::uint16_t
	{
		Unknown,
		Filled,
		PartiallyFilled,
		NotFilled
	};

	// TODO Start filling OrderStatus

	inline constexpr const char* GetCoreLogPath() noexcept
	{
		return "MicroExCore.log";
	}

	class Trade;

	using TradeCallbackFunc = std::function<void(Trade&)>;

	company_id_t MICROEX_API InvalidCompanyID() noexcept;

} // namespace MicroEx
