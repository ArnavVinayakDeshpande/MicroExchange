#include <engine/OrderBook.h>
#include <stdexcept>

namespace MicroEx
{

	OrderBook::OrderBook()
		:
		m_Asks(),
		m_Bids(),
		m_OrderLocations()
	{
		// TODO Log creation
	}

	OrderBook::~OrderBook()
	{
		// TODO Log destruction
	}

	std::optional<price_t> OrderBook::GetBestAskPrice() const
	{
		if (m_Asks.empty())
			return std::nullopt;

		return m_Asks.begin()->first;
	}

	std::optional<price_t> OrderBook::GetBestBidPrice() const
	{
		if (m_Bids.empty())
			return std::nullopt;

		return m_Bids.begin()->first;
	}

	std::optional<Order> OrderBook::GetBestAsk() const
	{
		if (m_Asks.empty())
			return std::nullopt;

		if (m_Asks.begin()->second.empty())
			return std::nullopt;

		return m_Asks.begin()->second.front();
	}

	std::optional<Order> OrderBook::GetBestBid() const
	{
		if (m_Bids.empty())
			return std::nullopt;

		if (m_Bids.begin()->second.empty())
			return std::nullopt;

		return m_Bids.begin()->second.front();
	}

	std::optional<std::vector<Order>> OrderBook::GetBestAskLevel() const
	{
		if (m_Asks.empty())
			return std::nullopt;

		if (m_Asks.begin()->second.empty())
			return std::nullopt;

		std::vector<Order> asks;
		asks.reserve(m_Asks.begin()->second.size());

		for (const auto& a : m_Asks.begin()->second)
		{
			asks.emplace_back(a);
		}

		return asks;
	}

	std::optional<std::vector<Order>> OrderBook::GetBestBidLevel() const
	{
		if (m_Bids.empty())
			return std::nullopt;

		if (m_Bids.begin()->second.empty())
			return std::nullopt;

		std::vector<Order> bids;
		bids.reserve(m_Bids.begin()->second.size());

		for (const auto& b : m_Bids.begin()->second)
		{
			bids.emplace_back(b);
		}

		return bids;
	}

	quantity_t OrderBook::GetBestAskOrderSize() const
	{
		if (m_Asks.empty())
			return 0;

		return m_Asks.begin()->second.size();
	}

	quantity_t OrderBook::GetBestBidOrderSize() const
	{
		if (m_Bids.empty())
			return 0;

		return m_Bids.begin()->second.size();
	}

	quantity_t OrderBook::GetBestAskDepth() const
	{
		if (m_Asks.empty())
			return 0;

		if (m_Asks.begin()->second.empty())
			return 0;

		quantity_t depth = 0;

		for (const auto& orders : m_Asks.begin()->second)
			depth += orders.Quantity;

		return depth;
	}

	quantity_t OrderBook::GetBestBidDepth() const
	{
		if (m_Bids.empty())
			return 0;

		if (m_Bids.begin()->second.empty())
			return 0;

		quantity_t depth = 0;

		for (const auto& orders : m_Bids.begin()->second)
			depth += orders.Quantity;

		return depth;
	}

	std::optional<std::vector<Order>> OrderBook::GetAskLevel(price_t price) const
	{
		if (m_Asks.empty())
			return std::nullopt;

		auto it = m_Asks.find(price);

		if (it == m_Asks.end())
			return std::nullopt;

		auto& book = it->second;

		std::vector<Order> asks;
		asks.reserve(book.size());

		for (const auto& a : book)
		{
			asks.emplace_back(a);
		}

		return asks;
	}

	std::optional<std::vector<Order>> OrderBook::GetBidLevel(price_t price) const
	{
		if (m_Bids.empty())
			return std::nullopt;

		auto it = m_Bids.find(price);

		if (it == m_Bids.end())
			return std::nullopt;

		auto& book = it->second;

		std::vector<Order> bids;
		bids.reserve(book.size());

		for (const auto& b : book)
		{
			bids.emplace_back(b);
		}

		return bids;
	}

	quantity_t OrderBook::GetAskOrderSize(price_t price) const
	{
		if (m_Asks.empty())
			return 0;

		auto it = m_Asks.find(price);

		if (it == m_Asks.end())
			return 0;

		return it->second.size();
	}

	quantity_t OrderBook::GetBidOrderSize(price_t price) const
	{
		if (m_Bids.empty())
			return 0;

		auto it = m_Bids.find(price);

		if (it == m_Bids.end())
			return 0;

		return it->second.size();
	}

	quantity_t OrderBook::GetAskDepth(price_t price) const
	{
		if (m_Asks.empty())
			return 0;

		auto it = m_Asks.find(price);

		if (it == m_Asks.end())
			return 0;

		quantity_t depth = 0;

		for (const auto& order : it->second)
			depth += order.Quantity;

		return depth;
	}

	quantity_t OrderBook::GetBidDepth(price_t price) const
	{
		if (m_Bids.empty())
			return 0;

		auto it = m_Bids.find(price);

		if (it == m_Bids.end())
			return 0;

		quantity_t depth = 0;

		for (const auto& order : it->second)
			depth += order.Quantity;

		return depth;
	}

	bool OrderBook::HasAsks() const
	{
		return !m_Asks.empty();
	}

	bool OrderBook::HasBids() const
	{
		return !m_Bids.empty();
	}

	bool OrderBook::HasAsksAtPrice(price_t price) const
	{
		if (m_Asks.empty())
			return false;

		return m_Asks.find(price) != m_Asks.end();
	}

	bool OrderBook::HasBidsAtPrice(price_t price) const
	{
		if (m_Bids.empty())
			return false;

		return m_Bids.find(price) != m_Bids.end();
	}

	bool OrderBook::InsertAsk(const Order& order)
	{
		// Sanitize
		if (order.Side != OrderSide::Seller)
			return false;

		// Check quantity
		if (order.Quantity == 0)
			return false;

		// Check price
		if (order.Price == 0) // For now we don't accept prices that are zero
			return false;

		// Add ask order
		m_Asks[order.Price].push_back(order);

		// Add to location storage
		m_OrderLocations[order.OrderID] = ms_OrderLocation(order.Side, order.Price, m_Asks[order.Price].end() - 1);

		return true;
	}

	bool OrderBook::InsertBid(const Order& order)
	{
		// Sanitize
		if (order.Side != OrderSide::Buyer)
			return false;

		// Check quantity
		if (order.Quantity == 0)
			return false;

		// Check price
		if (order.Price == 0) // For now we don't accept prices that are zero
			return false;

		// Add bid order
		m_Bids[order.Price].push_back(order);

		// Add to location storage
		m_OrderLocations[order.OrderID] = ms_OrderLocation(order.Side, order.Price, m_Bids[order.Price].end() - 1);

		return true;
	}

	bool OrderBook::CancelAsk(order_id_t orderID)
	{
		if (m_Asks.empty())
			return false; // No asks, nothing to cancel

		auto locationIt = m_OrderLocations.find(orderID); 

		if (locationIt == m_OrderLocations.end())
			return false; // ID doesn't exist

		auto& orderLocation = locationIt->second;

		if (orderLocation.Side != OrderSide::Seller)
			return false;

		auto orderIt = m_Asks.find(orderLocation.Price);

		if (orderIt == m_Asks.end())
			return false; // No order queue for price

		auto& queueBook = orderIt->second;

		queueBook.erase(orderLocation.It);
		m_OrderLocations.erase(locationIt);

		// If queueBook is empty now, erase t he price level
		if (queueBook.empty())
			m_Asks.erase(orderIt);

		return true;
	}

	bool OrderBook::CancelBid(order_id_t orderID)
	{
		if (m_Bids.empty())
			return false; // No asks, nothing to cancel

		auto locationIt = m_OrderLocations.find(orderID);

		if (locationIt == m_OrderLocations.end())
			return false; // ID doesn't exist

		auto& orderLocation = locationIt->second;

		if (orderLocation.Side != OrderSide::Buyer)
			return false;

		auto orderIt = m_Bids.find(orderLocation.Price);

		if (orderIt == m_Bids.end())
			return false; // No order queue for price

		auto& queueBook = orderIt->second;

		queueBook.erase(orderLocation.It);
		m_OrderLocations.erase(locationIt);

		// If queuebook is empty now, erase the price level
		if (queueBook.empty())
			m_Bids.erase(orderIt);

		return true;
	}

	quantity_t OrderBook::ConsumeBestAsk(quantity_t quantity)
	{
		if (!quantity)
			return 0;

		quantity_t totalQuantityConsumed = 0;

		while (quantity > 0)
		{
			// If there are no asks to consume, return
			if (m_Asks.empty())
				return totalQuantityConsumed;

			auto& bestAskLevel = m_Asks.begin()->second;

			// If there are asks but no more orders in the current price level
			// remove the price level and return
			if (bestAskLevel.empty())
			{
				// Remove price level
				this->RemoveBestAskLevel();
				return totalQuantityConsumed;
			}

			auto& bestAsk = bestAskLevel.front();

			// Check if quantity to be consumed is less than quantity avalaible
			if (quantity < bestAsk.Quantity)
			{
				// All quantity requirements are met, and there is left over quantity
				// in best ask so we update best ask quantity and we can just break the loop
				totalQuantityConsumed += quantity;
				bestAsk.Quantity -= quantity;
				break;
			}

			// Quantity required is equal to or greater than quantity avaiable for consumption
			// Reduce quantity asked
			quantity -= bestAsk.Quantity;

			// Increase quantity consumed
			totalQuantityConsumed += bestAsk.Quantity;

			// Pop the exhausted best ask
			this->RemoveBestAskOrder();
		}

		return totalQuantityConsumed;
	}

	quantity_t OrderBook::ConsumeBestBid(quantity_t quantity)
	{
		if (!quantity)
			return 0;

		quantity_t totalQuantityConsumed = 0;

		while (quantity > 0)
		{
			// If there are no bids to consume, return
			if (m_Bids.empty())
				return totalQuantityConsumed;

			auto& bestBidLevel = m_Bids.begin()->second;

			// If there are bids but no more orders in the current price level
			// remove the price level and return
			if (bestBidLevel.empty())
			{
				// Remove price level
				this->RemoveBestBidLevel();
				return totalQuantityConsumed;
			}

			auto& bestBid = bestBidLevel.front();

			// Check if quantity to be consumed is less than quantity avalaible
			if (quantity < bestBid.Quantity)
			{
				// All quantity requirements are met, and there is left over quantity
				// in best bid so we update best bids quantity and we can just break the loop
				totalQuantityConsumed += quantity;
				bestBid.Quantity -= quantity;
				break;
			}

			// Quantity required is equal to or greater than quantity avaiable for consumption
			// Reduce quantity bid
			quantity -= bestBid.Quantity;

			// Increase quantity consumed
			totalQuantityConsumed += bestBid.Quantity;

			// Pop the exhausted best bid
			this->RemoveBestBidOrder();
		}

		return totalQuantityConsumed;
	}

	void OrderBook::RemoveBestAskOrder()
	{
		// Make sure ask isn't empty
		if (m_Asks.empty())
			return;

		auto bestAsksIt = m_Asks.begin(); 

		// If the best ask level is empty, just pop the price
		if (bestAsksIt->second.empty())
		{
			m_Asks.erase(bestAsksIt);

			// Here we don't need to update OrderLocations since the ask level was empty already
			return;
		}

		// Pop the best ask
		// Get the order id
		order_id_t id = bestAsksIt->second.front().OrderID;

		// Update order locations
		auto it = m_OrderLocations.find(id);

		if (it == m_OrderLocations.end())
		{
			// Something has seriously gone wrong
			// log into error
			// return
			throw std::runtime_error("Corrupted Order Location Book and Asks Order Book");
		}

		// Pop
		bestAsksIt->second.pop_front();
		
		// Remove from order locations
		m_OrderLocations.erase(it);

		// Check if price level is empty now
		if (bestAsksIt->second.empty())
		{
			m_Asks.erase(bestAsksIt);

			// We don't need to return here since we should log and then return after removal
		}

		// Log

		return;
	}

	void OrderBook::RemoveBestBidOrder()
	{
		// Make sure bid isn't empty
		if (m_Bids.empty())
			return;

		auto bestBidsIt = m_Bids.begin();

		// If the best bid level is empty, just pop the price
		if (bestBidsIt->second.empty())
		{
			m_Bids.erase(bestBidsIt);

			// Here we don't need to update OrderLocations since the bid level was empty already
			return;
		}

		// Pop the best bid
		// Get the order id
		order_id_t id = bestBidsIt->second.front().OrderID;

		// Update order locations
		auto it = m_OrderLocations.find(id);

		if (it == m_OrderLocations.end())
		{
			// Something has seriously gone wrong
			// log into error
			// return
			throw std::runtime_error("Corrupted Order Location Book and Bids Order Book");
		}

		// pop
		bestBidsIt->second.pop_front();

		// Remove from order locations
		m_OrderLocations.erase(it);

		// Check if price level is empty now
		if (bestBidsIt->second.empty())
		{
			m_Bids.erase(bestBidsIt);

			// We don't need to return here since we should log and then return after removal
		}

		// Log

		return;
	}

	void OrderBook::RemoveBestAskLevel()
	{
		// Make sure asks isn't empty
		if (m_Asks.empty())
			return;

		auto bestAsksIt = m_Asks.begin();

		const price_t price = bestAsksIt->first;

		while (!m_Asks.empty() && m_Asks.begin()->first == price)
			this->RemoveBestAskOrder();
	}

	void OrderBook::RemoveBestBidLevel()
	{
		// Make sure bids isn't empty
		if (m_Bids.empty())
			return;

		auto bestBidsIt = m_Bids .begin();

		const price_t price = bestBidsIt->first;

		while (!m_Bids.empty() && m_Bids.begin()->first == price)
			this->RemoveBestBidOrder();
	}

	void OrderBook::RemoveAskLevel(price_t price)
	{
		auto it = m_Asks.find(price);

		if (it == m_Asks.end())
			return;

		while (m_Asks.find(price) != m_Asks.end())
		{
			auto& q = m_Asks[price];

			this->CancelAsk(q.front().OrderID);
		}
	}

	void OrderBook::RemoveBidLevel(price_t price)
	{
		auto it = m_Bids.find(price);

		if (it == m_Bids.end())
			return;

		while (m_Bids.find(price) != m_Bids.end())
		{
			auto& q = m_Bids[price];

			this->CancelBid(q.front().OrderID);
		}
	}

}