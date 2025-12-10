#include <engine/OrderBook.h>
#include <UUIDGenerator.h>
#include <stdexcept>

namespace MicroEx
{

	OrderBook::OrderBook(const TradeCallbackFunc& tradeCallbackFunc)
		:
		m_Bids(),
		m_Asks(),
		m_TradeCallbackFunc(tradeCallbackFunc)
	{
		if (!m_TradeCallbackFunc)
		{
			throw std::invalid_argument("Trade callback function cannot be null.");
		}
	}

	OrderBook::~OrderBook()
	{
		m_Bids.clear();
		m_Asks.clear();
	}

	std::optional<Price> OrderBook::GetBestBidValue() const
	{
		if (m_Bids.empty())
			return std::nullopt;

		return m_Bids.begin()->first;
	}

	std::optional<Price> OrderBook::GetBestAskValue() const
	{
		if (m_Asks.empty())
			return std::nullopt;

		return m_Asks.begin()->first;
	}

	std::optional<Quantity> OrderBook::GetBestBidAmount() const
	{
		if (m_Bids.empty())
			return std::nullopt;

		const auto& bestBidOrders = m_Bids.begin()->second;

		Quantity amount = 0;

		for (const auto& order : bestBidOrders)
			amount += order.Amount;

		return amount;
	}

	std::optional<Quantity> OrderBook::GetBestAskAmount() const
	{
		if (m_Asks.empty())
			return 0;

		const auto& bestAskOrders = m_Asks.begin()->second;
		
		Quantity amount = 0;
	
		for (const auto& order : bestAskOrders)
			amount += order.Amount;
		
		return amount;
	}

	std::optional<std::vector<Order>> OrderBook::GetBestBids() const
	{
		return this->GetBidsAtPrice(m_Bids.begin()->first);
	}

	std::optional<std::vector<Order>> OrderBook::GetBestAsks() const
	{
		return this->GetAsksAtPrice(m_Asks.begin()->first);
	}

	std::optional<std::vector<Order>> OrderBook::GetBidsAtPrice(double price) const
	{
		if (price <= 0.0)
			return std::nullopt;

		auto it = m_Bids.find(price);

		if (it == m_Bids.end())
			return std::nullopt;

		return std::vector<Order>(it->second.begin(), it->second.end());
	}

	std::optional<std::vector<Order>> OrderBook::GetAsksAtPrice(double price) const
	{
		if (price <= 0.0)
			return std::nullopt;

		auto it = m_Asks.find(price);

		if (it == m_Asks.end())
			return std::nullopt;

		return std::vector<Order>(it->second.begin(), it->second.end());
	}

	OrderID OrderBook::PlaceOrder(const Order& order)
	{

	}

	void OrderBook::RemoveOrder(OrderID orderId)
	{
	}

}