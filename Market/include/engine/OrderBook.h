/**
 * @file Market/include/engine/OrderBook.h
 * @author Arnav Deshpande
 * 
 * This file contains implementation for Orders and for the OrderBook used in the market.
 */

#pragma once

//#error stop_here
#include <core/Core.h>
#include <Timestamp.h>
#include <core/Price.h>

#include <map>
#include <unordered_map>
#include <deque>
#include <optional>
#include <vector>
#include <utility>

namespace MicroEx
{

	/**
	 * @struct Order
	 * 
	 * @brief Represents an Order in the Market.
	 * 
	 * Represents and stores metadata of an order that is placed in the market.
	 */
	struct MICROEX_API Order final
	{
		/// Identifer of the order.
		order_id_t OrderID;

		/// Identifer of the company whose securities the order has been placed for.
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
	class MICROEX_API OrderBook final
	{
	public:
		explicit OrderBook() noexcept;
		~OrderBook() noexcept;

		uuid_t GetUUID() const noexcept
		{
			return m_UUID;
		}

		std::optional<price_t> GetBestAskPrice() const noexcept;
		std::optional<price_t> GetBestBidPrice() const noexcept;

		std::optional<Order> GetBestAskOrder() const noexcept;
		std::optional<Order> GetBestBidOrder() const noexcept;

		std::vector<Order> GetBestAskLevel() const noexcept;
		std::vector<Order> GetBestBidLevel() const noexcept;

		quantity_t GetBestAskOrderSize() const noexcept;
		quantity_t GetBestBidOrderSize() const noexcept;

		quantity_t GetBestAskDepth() const noexcept;
		quantity_t GetBestBidDepth() const noexcept;

		std::vector<Order> GetAskLevel(price_t price) const noexcept;
		std::vector<Order> GetBidLevel(price_t price) const noexcept;

		quantity_t GetAskOrderSize(price_t price) const noexcept;
		quantity_t GetBidOrderSize(price_t price) const noexcept;

		quantity_t GetAskDepth(price_t price) const noexcept;
		quantity_t GetBidDepth(price_t price) const noexcept;

		std::optional<Order> FindOrder(order_id_t orderID) const noexcept;

		bool HasAsks() const noexcept;
		bool HasBids() const noexcept;

		bool HasAsksAtPrice(price_t price) const noexcept;
		bool HasBidsAtPrice(price_t price) const noexcept;

		bool InsertAsk(const Order& order) noexcept;
		bool InsertBid(const Order& order) noexcept;

		bool CancelAsk(order_id_t orderID) noexcept;
		bool CancelBid(order_id_t orderID) noexcept;

		quantity_t ConsumeBestAsk(quantity_t quantity) noexcept;
		quantity_t ConsumeBestBid(quantity_t quantity) noexcept;

		void RemoveBestAskOrder() noexcept;
		void RemoveBestBidOrder() noexcept;

		void RemoveBestAskLevel() noexcept;
		void RemoveBestBidLevel() noexcept;

		void RemoveAskLevel(price_t price) noexcept;
		void RemoveBidLevel(price_t price) noexcept;

		void RemoveAllAsks() noexcept;
		void RemoveAllBids() noexcept;

		void RemoveAllOrders() noexcept;

		/*void SanitiationCheck() noexcept;
		bool Validate() noexcept;*/

	private:
		struct ms_OrderLocation final
		{
			OrderSide Side;
			price_t Price;
			std::optional<std::deque<Order>::iterator> It;

			ms_OrderLocation() noexcept // TODO Proper Default Constructor
				:
				Side(OrderSide::Buyer),
				Price(ValueTypes::ZeroPrice),
				It(std::nullopt) // Make It an optional
			{
			}

			explicit ms_OrderLocation(OrderSide side, price_t price, std::optional<std::deque<Order>::iterator> it) noexcept
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
		uuid_t m_UUID;
	};

}
