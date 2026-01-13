/**
 * @file Market/include/engine/MatchingEngine.h
 * @author Arnav Deshpande
 * 
 * This file contains declarations for the Matching Engine.
 */

#pragma once

#include <core/Core.h>
#include <engine/OrderBook.h>
#include <market/CompanyRegistry.h>

#include <map>
#include <optional>

namespace MicroEx
{

	/**
	 * @class MatchEngine
	 * 
	 * @brief Executes orders of all different types depending on matching conditions.
	 * 
	 * This class contains implementation for the Matching Engine of the market. It matches orders of all types and both sides,
	 * according to their matching requirements. It stores partially filled orders if permitted by the order type, and can be used to query
	 * status of existing orders in the order book.
	 * It also contains the Market Prices of securities traded according to the last executed order.
	 * 
	 * This class is not inheritable.
	 */
	class MICROEX_API MatchEngine final
	{
	public:
		/**
		 * @func Constructor
		 * 
		 * @par registry Registry of the companies trading securities using this MatchEngine.
		 * 
		 * CompanyRegistry and MarketPrices have the invariance of equality. All the companies in the registry must be included in the MatchEngine,
		 * and if the original registry is modified in any way, the invariance is broken and must be dealt with.
		 * 
		 * TODO Add a way to deal with the invariance
		 */
		MatchEngine(const CompanyRegistry& registry);

		/**
		 * Destructor
		 */
		~MatchEngine();

		/**
		 * @func PlaceOrder
		 * @brief Places an order in the market.
		 * 
		 * @par order Order to be placed.
		 * 
		 * This function places an order that is to be executed according to the parameters of the order given. In case of limit orders, 
		 * this function automatically executes the order and adds (if exists) the partially filled order to the OrderBook.
		 */
		void PlaceOrder(Order order);

		/**
		 * @func GetMarketPrice
		 * 
		 * @par companyID Identifer of the company for querying.
		 * 
		 * @return std::optional<price_t>
		 * @returns Market price of the company if company exists in the MatchEngine.
		 * 
		 * If the company does not exist in the MatchEngine, std::nullopt is returned. In the case company does exist in the registry used
		 * to create the MatchEngine, but does not exist in the internal storage of market prices, invariance is broken and market is in 
		 * corrupted state.
		 * 
		 * TODO Add a way to deal with this invariance
		 */
		std::optional<price_t> GetMarketPrice(company_id_t companyID) const;

		/**
		 * @func operator[]
		 * 
		 * @par companyID Identifer of the company for querying.
		 * 
		 * @return std::optional<price_t>
		 * @returns Market price of the company if company exists in the MatchEngine.
		 * 
		 * See @func MicroEx::MatchEngine::GetMarketPrice for details.
		 */
		std::optional<price_t> operator[](company_id_t companyID) const;

		/**
		 * @func FindOrder
		 * 
		 * @par orderID Identifer of the order for querying.
		 * 
		 * @return std::optional<Order>
		 * @returns Order, if exists, that is asked for.
		 * 
		 * If the order does not exist in the OrderBook, std::nullopt is return. Else the current state of Order is returned.
		 * The following conditions result in absence of order:
		 * 		1. Order is a Market Order
		 * 		2. Order is completed 
		 * 
		 * @note Orders are stored in their current state, not their original state. If a order has been partially filled, the order returned 
		 * and stored reflects the partial fill. Order ID is immutable for change in order state i.e. partial fill of order does not change the
		 * order ID, therefore no new order is generated on partial fill of an order. There is no mechanism to query the original state
		 * of the order.
		 */
		std::optional<Order> FindOrder(order_id_t orderID) const;

		/**
		 * @func operator()
		 * 
		 * @par orderID Identifer of the order for querying.
		 * 
		 * @return std::optional<Order>
		 * @returns Order, if exists, that is asked for.
		 * 
		 * See @func MicroEx::MatchEngine:FindOrder for details.
		 */
		std::optional<Order> operator()(order_id_t orderID) const;

	private:
		/**
		 * @func m_PlaceLimitAsk
		 * 
		 * @par order Reference to order placed.
		 * 
		 * @brief Executes a limit ask order and adds remaining order (if exists) to the Order Book.
		 */
		void m_PlaceLimitAsk(Order& order);

		/**
		 * @func m_PlaceLimitBid
		 * 
		 * @par order Reference to order placed.
		 * 
		 * @brief Executes a limit bid order and adds remaining order (if exists) to the Order Book.
		 */
		void m_PlaceLimitBid(Order& order);

		/**
		 * @func m_PlaceMarketAsk
		 * 
		 * @par order Reference to order placed.
		 * 
		 * @brief Executes the order at market price.
		 */
		void m_PlaceMarketAsk(Order& order);

		/**
		 * @func m_PlaceMarketBid
		 * 
		 * @par order Reference to order placed.
		 * 
		 * @brief Executes the order at market price.
		 */
		void m_PlaceMarketBid(Order& order);

	private:
		/// Order book 
		OrderBook m_OrderBook;

		/// Registry of market prices of companies TODO Expand to add multiple securities.
		std::map<company_id_t, price_t> m_MarketPrices;
	};

} // namespace MicroEx
