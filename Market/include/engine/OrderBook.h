#pragma once

#include <engine/Core.h>
#include <Timestamp.h>
#include <unordered_map>
#include <deque>

namespace MicroEx
{

	struct MICROEX_API Order
	{
		OrderID Id;
		StockID StockId;
		OrderSide Side;
		OrderType Type;
		double Price;
		uint32_t Quantity;
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

		double GetBestBidPrice() const;

		double GetBestAskPrice() const;

		uint32_t GetBestBidQuantity() const;
		uint32_t GetBestAskQuantity() const;

		std::vector<Order> GetBestBids() const;
		std::vector<Order> GetBestAsks() const;

		std::vector<Order> GetBidsAtPrice(double price) const;
		std::vector<Order> GetAsksAtPrice(double price) const;

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