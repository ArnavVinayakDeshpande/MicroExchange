#pragma once

#include <engine/Core.h>
#include <Timestamp.h>
#include <map>
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
		OrderBook(const TradeCallbackFunc& tradeCallbackFunc);
		~OrderBook();

		const TradeCallbackFunc& GetTradeCallbackFunc() const
		{
			return m_TradeCallbackFunc;
		}

		std::optional<price_t> GetBestBidValue() const;
		std::optional<price_t> GetBestAskValue() const;

		std::optional<quantity_t> GetBestBidAmount() const;
		std::optional<quantity_t> GetBestAskAmount() const;

		std::optional<std::vector<Order>> GetBestBids() const;
		std::optional<std::vector<Order>> GetBestAsks() const;

		std::optional<std::vector<Order>> GetBidsAtPrice(price_t price_t) const;
		std::optional<std::vector<Order>> GetAsksAtPrice(price_t price_t) const;

		void SetTradeCallbackFunc(const TradeCallbackFunc& tradeCallbackFunc)
		{
			m_TradeCallbackFunc = tradeCallbackFunc;
		}

		order_id_t PlaceOrder(const Order& order);
		void RemoveOrder(order_id_t orderID);

	private:
		order_id_t m_MatchBidOrder(Order order);
		order_id_t m_MatchAskOrder(Order order);

	private:
		std::map<price_t, std::deque<Order>, std::greater<price_t>> m_Bids;
		std::map<price_t, std::deque<Order>, std::less<price_t>> m_Asks;
		TradeCallbackFunc m_TradeCallbackFunc;
	};

}