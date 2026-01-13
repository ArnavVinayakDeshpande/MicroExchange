/**
 * @file Market/include/engine/OrderBook.h
 * @author Arnav Deshpande
 * 
 * This file contains implementation for Orders and for the OrderBook used in the market.
 */

#pragma once

#include <core/Core.h>
#include <Timestamp.h>
#include <core/Price.h>

#include <map>
#include <unordered_map>
#include <deque>
#include <optional>

namespace MicroEx
{

	/**
	 * @struct Order
	 * 
	 * @brief Represents an Order in the Market.
	 * 
	 * Represents and stores metadata of an order that is placed in the market.
	 */
	struct MICROEX_API Order
	{
		/// Identifer of the order.
		order_id_t OrderID;

		// Identifer of the company whose securities the order has been placed for.
		company_id_t CompanyID;

		/// Side of the buyer i.e. Seller or Buyer.
		OrderSide Side;

		/// Type of the order, ex. Limit, Market.
		OrderType Type;

		/// Price at which the order is to be executed at (ignored for Market Orders).
		price_t Price;

		/// Quantity of securities the order has been placed for.
		quantity_t Quantity;

		/// Datetime when the order was placed.
		Timestamp TimePlaced;
	};

	/**
	 * @class OrderBook
	 * 
	 * @brief Implementation of the order book of the market.
	 * 
	 * This class implements the functioning of an order book in the market. 
	 */
	class MICROEX_API OrderBook
	{
	public:
		OrderBook();
		~OrderBook();

		std::optional<price_t> GetBestAskPrice() const;
		std::optional<price_t> GetBestBidPrice() const;

		std::optional<Order> GetBestAskOrder() const;
		std::optional<Order> GetBestBidOrder() const;

		std::optional<std::vector<Order>> GetBestAskLevel() const;
		std::optional<std::vector<Order>> GetBestBidLevel() const;

		quantity_t GetBestAskOrderSize() const;
		quantity_t GetBestBidOrderSize() const;

		quantity_t GetBestAskDepth() const;
		quantity_t GetBestBidDepth() const;

		std::optional<std::vector<Order>> GetAskLevel(price_t price) const;
		std::optional<std::vector<Order>> GetBidLevel(price_t price) const;

		quantity_t GetAskOrderSize(price_t price) const;
		quantity_t GetBidOrderSize(price_t price) const;

		quantity_t GetAskDepth(price_t price) const;
		quantity_t GetBidDepth(price_t price) const;

		std::optional<Order> FindOrder(order_id_t orderID) const;

		bool HasAsks() const;
		bool HasBids() const;

		bool HasAsksAtPrice(price_t price) const;
		bool HasBidsAtPrice(price_t price) const;

		bool InsertAsk(const Order& order);
		bool InsertBid(const Order& order);

		bool CancelAsk(order_id_t orderID);
		bool CancelBid(order_id_t orderID);

		quantity_t ConsumeBestAsk(quantity_t quantity);
		quantity_t ConsumeBestBid(quantity_t quantity);

		void RemoveBestAskOrder();
		void RemoveBestBidOrder();

		void RemoveBestAskLevel();
		void RemoveBestBidLevel();

		void RemoveAskLevel(price_t price);
		void RemoveBidLevel(price_t price);

		void SanitiationCheck();
		bool Validate() noexcept;

	private:
		struct ms_OrderLocation
		{
			OrderSide Side;
			price_t Price;
			std::optional<std::deque<Order>::iterator> It;

			ms_OrderLocation() // TODO Proper Default Constructor
				:
				Side(OrderSide::Buyer),
				Price(0),
				It(std::nullopt) // Make It an optional
			{
			}

			ms_OrderLocation(OrderSide side, price_t price, std::optional<std::deque<Order>::iterator> it)
				:
				Side(side),
				Price(price),
				It(std::move(it))
			{
			}
		};

	private:
		using eng_price_t = double;

	private:
		std::map<eng_price_t, std::deque<Order>, std::less<eng_price_t>> m_Asks;
		std::map<eng_price_t, std::deque<Order>, std::greater<eng_price_t>> m_Bids;
		std::unordered_map<order_id_t, ms_OrderLocation> m_OrderLocations;
	};

}