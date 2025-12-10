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

	double OrderBook::GetBestBidPrice() const
	{
		if (m_Bids.empty())
			return 0.0;
		return m_Bids.begin()->first;
	}

	double OrderBook::GetBestAskPrice() const
	{
		if (m_Asks.empty())
			return 0.0;
		return m_Asks.begin()->first;
	}

	uint32_t OrderBook::GetBestBidQuantity() const
	{
		if (m_Bids.empty())
			return 0;

		const auto& bestBidOrders = m_Bids.begin()->second;

		uint32_t quantity = 0;

		for (const auto& order : bestBidOrders)
			quantity += order.Quantity;

		return quantity;
	}

	uint32_t OrderBook::GetBestAskQuantity() const
	{
		if (m_Asks.empty())
			return 0;

		const auto& bestAskOrders = m_Asks.begin()->second;
		
		uint32_t quantity = 0;
	
		for (const auto& order : bestAskOrders)
			quantity += order.Quantity;
		
		return quantity;
	}

	std::vector<Order> OrderBook::GetBestBids() const
	{
		return this->GetBidsAtPrice(this->GetBestBidPrice());
	}

	std::vector<Order> OrderBook::GetBestAsks() const
	{
		return this->GetAsksAtPrice(this->GetBestAskPrice());
	}

	std::vector<Order> OrderBook::GetBidsAtPrice(double price) const
	{
		if (price <= 0.0)
			return {};

		auto it = m_Bids.find(price);

		if (it == m_Bids.end())
			return {};

		return std::vector<Order>(it->second.begin(), it->second.end());
	}

	std::vector<Order> OrderBook::GetAsksAtPrice(double price) const
	{
		if (price <= 0.0)
			return {};

		auto it = m_Asks.find(price);

		if (it == m_Bids.end())
			return {};

		return std::vector<Order>(it->second.begin(), it->second.end());
	}

	OrderID OrderBook::PlaceOrder(const Order& order)
	{

	}

	void OrderBook::RemoveOrder(OrderID orderId)
	{
	}

}