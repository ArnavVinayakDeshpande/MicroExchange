#pragma once

#include <engine/Core.h>
#include <Timestamp.h>
#include <map>
#include <unordered_map>
#include <deque>
#include <optional>

namespace MicroEx
{

	struct MICROEX_API Order
	{
		order_id_t OrderID;
		company_id_t CompanyID;
		OrderSide Side;
		OrderType Type;
		price_t Price;
		quantity_t Quantity;
		Timestamp TimePlaced;
	};

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
		std::map<price_t, std::deque<Order>, std::less<price_t>> m_Asks;
		std::map<price_t, std::deque<Order>, std::greater<price_t>> m_Bids;
		std::unordered_map<order_id_t, ms_OrderLocation> m_OrderLocations;
	};

}