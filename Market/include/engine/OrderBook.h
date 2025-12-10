#pragma once

#include <engine/Core.h>
#include <Timestamp.h>
#include <unordered_map>
#include <deque>
#include <optional>

namespace MicroEx
{

	struct MICROEX_API Order
	{
		OrderID Id;
		StockID StockId;
		OrderSide Side;
		OrderType Type;
		Price Value;
		Quantity Amount;
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

		std::optional<Price> GetBestBidValue() const;
		std::optional<Price> GetBestAskValue() const;

		std::optional<Quantity> GetBestBidAmount() const;
		std::optional<Quantity> GetBestAskAmount() const;

		std::optional<std::vector<Order>> GetBestBids() const;
		std::optional<std::vector<Order>> GetBestAsks() const;

		std::optional<std::vector<Order>> GetBidsAtPrice(double price) const;
		std::optional<std::vector<Order>> GetAsksAtPrice(double price) const;

		void SetTradeCallbackFunc(const TradeCallbackFunc& tradeCallbackFunc)
		{
			m_TradeCallbackFunc = tradeCallbackFunc;
		}

		OrderID PlaceOrder(const Order& order);
		void RemoveOrder(OrderID orderId);

	private:
		std::unordered_map<double, std::deque<Order>, std::greater<double>> m_Bids;
		std::unordered_map<double, std::deque<Order>> m_Asks;
		TradeCallbackFunc m_TradeCallbackFunc;
	};

}