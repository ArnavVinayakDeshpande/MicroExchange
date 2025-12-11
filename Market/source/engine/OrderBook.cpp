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

	std::optional<price_t> OrderBook::GetBestBidValue() const
	{
		if (m_Bids.empty())
			return std::nullopt;

		return m_Bids.begin()->first;
	}

	std::optional<price_t> OrderBook::GetBestAskValue() const
	{
		if (m_Asks.empty())
			return std::nullopt;

		return m_Asks.begin()->first;
	}

	std::optional<quantity_t> OrderBook::GetBestBidAmount() const
	{
		if (m_Bids.empty())
			return std::nullopt;

		const auto& bestBidOrders = m_Bids.begin()->second;

		quantity_t quantity = 0;

		for (const auto& order : bestBidOrders)
			quantity += order.Quantity;

		return quantity;
	}

	std::optional<quantity_t> OrderBook::GetBestAskAmount() const
	{
		if (m_Asks.empty())
			return 0;

		const auto& bestAskOrders = m_Asks.begin()->second;
		
		quantity_t quantity = 0;
	
		for (const auto& order : bestAskOrders)
			quantity += order.Quantity;
		
		return quantity;
	}

	std::optional<std::vector<Order>> OrderBook::GetBestBids() const
	{
		return this->GetBidsAtPrice(m_Bids.begin()->first);
	}

	std::optional<std::vector<Order>> OrderBook::GetBestAsks() const
	{
		return this->GetAsksAtPrice(m_Asks.begin()->first);
	}

	std::optional<std::vector<Order>> OrderBook::GetBidsAtPrice(price_t price) const
	{
		if (price <= 0.0)
			return std::nullopt;

		auto it = m_Bids.find(price);

		if (it == m_Bids.end())
			return std::nullopt;

		return std::vector<Order>(it->second.begin(), it->second.end());
	}

	std::optional<std::vector<Order>> OrderBook::GetAsksAtPrice(price_t price) const
	{
		if (price <= 0.0)
			return std::nullopt;

		auto it = m_Asks.find(price);

		if (it == m_Asks.end())
			return std::nullopt;

		return std::vector<Order>(it->second.begin(), it->second.end());
	}

	order_id_t OrderBook::PlaceOrder(const Order& order)
	{
		// Assume order is always valid, and MatchEngine handles invalid orders

		// For a bid
		if (order.Side == OrderSide::Buyer)
		{
			m_MatchBidOrder(order);
		}
		// For a 
		else if (order.Side == OrderSide::Seller)
		{
			m_MatchAskOrder(order);
		}

		return order.OrderID;
	}

	void OrderBook::RemoveOrder(order_id_t orderID)
	{
	}

	order_id_t OrderBook::m_MatchBidOrder(Order order)
	{
		// Ensure order is really a bid
		if (order.Side != OrderSide::Buyer)
			return order.OrderID; // Error queue

		// Ensure quantity of bid is greater than zero
		while (order.Quantity > 0) // TODO Add a max iteration (max quantity in bids)
		{
			// If there are no sellers, log and exit
			if (m_Asks.empty())
			{
				// No sell orders, add the order to the bid order book
				m_Bids[order.Price].push_back(order);
				return order.OrderID;
			}

			auto bestAskPrice = m_Asks.begin()->first;
			auto& bestAskOrderBook = m_Asks.begin()->second;

			// Check if bid is greater than best ask price
			if (order.Price < bestAskPrice)
			{
				// No trade takes place, add this to the bids queuebook
				m_Bids[order.Price].push_back(order);

				// Return without any transactions
				return order.OrderID;
			}

			// Check if there are any sellers at the best ask price
			if (bestAskOrderBook.empty())
			{
				// There are no sellers at this price point, remove this from the asks and continue the loop
				// to the next price available 
				m_Asks.erase(bestAskPrice);
				continue;
			}

			// Buyers at this stage exist
			// Get the best ask (FIFO)
			auto& bestAskOrder = bestAskOrderBook.front();

			// Quantity of securities traded is the minimum of quantity and avaiable stocks
			quantity_t quantityToBeTraded = std::min(order.Quantity, bestAskOrder.Quantity);

			// Commit trade at this quantity and bestAskOrder.Price

			// Substract the quantityToBeTraded from order.Quantity
			order.Quantity -= quantityToBeTraded;

			// If quantity to be traded is less than quantity in bestAskOrder, we need to substract that 
			// from the bestAskOrder
			if (quantityToBeTraded < bestAskOrder.Quantity)
			{
				bestAskOrder.Quantity -= quantityToBeTraded;
				break; // We can break here since trade has happened on entire bid order, thus no entry to bids and no reordering of asks
			}

			// If quantityToBeTraded is equal to or more than quantity in bestAskOrder, we need to 
			// pop the list to get the next best order
			bestAskOrderBook.pop_front();

			// If this pop makes the orderbook empty, we check it in the next iteration
			// if it exists we complete the trade
			// if it doesn't we pop the price and get the quebook for the next price
			// this cycle continues until we either have asks that do not match the bids, or all ask orders are finished
			// upon which if any quantity of the bids remain, we just add it to the bid order.
		}
	}

	order_id_t OrderBook::m_MatchAskOrder(Order order)
	{

	}

}