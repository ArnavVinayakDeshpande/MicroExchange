#include <engine/MatchingEngine.h>

namespace MicroEx
{

	MatchEngine::MatchEngine(const CompanyRegistry& registry)
		:
		m_OrderBook(),
		m_MarketPrices()
	{
		auto company_ids = registry.GetAllCompanyIDs();

		if (!company_ids)
			throw std::runtime_error("Invalid registry"); // TODO Add asserts/error management, exception temporary

		for (const auto& id : company_ids.value())
			m_MarketPrices[id] = 0.0; // TODO Market Price initialization by excel file
	}

	MatchEngine::~MatchEngine()
	{
		// TODO Logging
	}

	void MatchEngine::PlaceOrder(Order order)
	{
		// Add a check for company id

		if (order.Quantity == 0 || order.Price == 0)
			return; // Logging

		if (order.Type == OrderType::Market)
		{
			if (order.Side == OrderSide::Seller)
				m_PlaceMarketAsk(order);
			else if (order.Side == OrderSide::Buyer)
				m_PlaceMarketBid(order);
		}
		else if (order.Type == OrderType::Limit)
		{
			if (order.Side == OrderSide::Seller)
				m_PlaceLimitAsk(order);
			else if (order.Side == OrderSide::Buyer)
				m_PlaceLimitBid(order);
		}
	}

	std::optional<price_t> MatchEngine::GetMarketPrice(company_id_t companyID) const
	{
		auto it = m_MarketPrices.find(companyID);

		
		//return it != m_MarketPrices.end() ? it->second : std::nullopt;
	
		if (it == m_MarketPrices.end())
			return std::nullopt;

		return it->second;
	}

	std::optional<price_t> MatchEngine::operator[](company_id_t companyID) const
	{
		return this->GetMarketPrice(companyID);
	}

	std::optional<Order> MatchEngine::FindOrder(order_id_t orderID) const
	{
		return m_OrderBook.FindOrder(orderID);
	}

	std::optional<Order> MatchEngine::operator()(order_id_t orderID) const
	{
		return this->FindOrder(orderID);
	}

	void MatchEngine::m_PlaceLimitAsk(Order& order)
	{
		quantity_t& quantity = order.Quantity;
		const price_t& price = order.Price;
		price_t currentMarketPrice = m_MarketPrices[order.CompanyID];

		while (quantity > 0)
		{
			// Check if we can match
			auto bestBidOpt = m_OrderBook.GetBestBidPrice();

			if (!bestBidOpt)
			{
				// log

				break;
			}

			// Best bid exists, compare it to price
			price_t bestBid = bestBidOpt.value();

			if (bestBid < price)
			{

				// log

				break;
			}

			// Place the order
			quantity_t quantityConsumed = m_OrderBook.ConsumeBestBid(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best bid

				// Since trade has happened, we update the market price
				currentMarketPrice = bestBid;
			}
			else
			{
				// LOGIC ERROR SINCE THIS BREAKS ORDER BOOK INVARIANCE
				throw std::logic_error("OrderBook invariance broken in asks.");
			}

			// Reduce the quantity
			quantity -= quantityConsumed;

			// If quantity consumed is zero, there are no more buy orders or no more buy orders
			// at this price level, so we don't exit and go to the next iteration to see if
			// we can match the next price level
		}

		// Now either all the quantity has been consumed, or 
		// there are no more sell orders, or
		// the best bid and ask don't match
		// if quantity exists, we add a bid order and exit the function
		if (quantity != 0)
			m_OrderBook.InsertAsk(order);

		// Set the market price
		m_MarketPrices[order.CompanyID] = currentMarketPrice;
	}

	void MatchEngine::m_PlaceLimitBid(Order& order)
	{
		quantity_t& quantity = order.Quantity;
		price_t price = order.Price;
		price_t currentMarketPrice = m_MarketPrices[order.CompanyID];

		while (quantity > 0)
		{
			// Check if we can match
			auto bestAskOpt = m_OrderBook.GetBestAskPrice();

			if (!bestAskOpt)
			{
				
				break;
			}

			// Best ask exists, compare it to price
			price_t bestAsk = bestAskOpt.value();

			if (price >= bestAsk)
			{

				// log

				break;
			}

			// Place the order
			quantity_t quantityConsumed = m_OrderBook.ConsumeBestAsk(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best ask
				

				// Since trade has happened, we update the market price
				currentMarketPrice = bestAsk;
			}
			else
			{
				// LOGIC ERROR SINCE THIS BREAKS ORDER BOOK INVARIANCE
				throw std::logic_error("OrderBook invariance broken in bids.");
			}

			// Reduce the quantity
			quantity -= quantityConsumed;

			// If quantity consumed is zero, there are no more sell orders or no more sell orders
			// at this price level, so we don't exit and go to the next iteration to see if
			// we can match the next price level
		}

		// Now either all the quantity has been consumed, or 
		// there are no more sell orders, or
		// the best ask and bid don't match
		// if quantity exists, we add a bid order and exit the function
		if (quantity != 0)
			m_OrderBook.InsertBid(order);

		// Set the market price
		m_MarketPrices[order.CompanyID] = currentMarketPrice;
	}

	void MatchEngine::m_PlaceMarketAsk(Order& order)
	{
		quantity_t& quantity = order.Quantity;
		price_t currentMarketPrice = m_MarketPrices[order.CompanyID];

		while (quantity > 0)
		{
			// Check if we can match
			auto bestBidOpt = m_OrderBook.GetBestBidPrice();

			if (!bestBidOpt)
			{
				// log

				break;
			}

			// Best bid exists, compare it to price
			price_t bestBid = bestBidOpt.value();

			// Place the order
			quantity_t quantityConsumed = m_OrderBook.ConsumeBestBid(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best bid

				// Since trade has happened, we update the market price
				currentMarketPrice = bestBid;
			}
			else
			{
				// LOGIC ERROR SINCE THIS BREAKS ORDER BOOK INVARIANCE
				throw std::logic_error("OrderBook invariance broken in asks.");
			}

			// Reduce the quantity
			quantity -= quantityConsumed;

			// If quantity consumed is zero, there are no more buy orders or no more buy orders
			// at this price level, so we don't exit and go to the next iteration to see if
			// we can match the next price level
		}

		// Now either all the quantity has been consumed, or 
		// there are no more sell orders, and we don't add it to orderbook

		// Set the market price
		m_MarketPrices[order.CompanyID] = currentMarketPrice;
	}

	void MatchEngine::m_PlaceMarketBid(Order& order)
	{
		quantity_t& quantity = order.Quantity;
		const price_t& price = order.Price;
		price_t currentMarketPrice = m_MarketPrices[order.CompanyID];

		while (quantity > 0)
		{
			// Check if we can match
			auto bestAskOpt = m_OrderBook.GetBestAskPrice();

			if (!bestAskOpt)
			{

				break;
			}

			// Best ask exists, compare it to price
			price_t bestAsk = bestAskOpt.value();

			// Place the order
			quantity_t quantityConsumed = m_OrderBook.ConsumeBestAsk(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best ask


				// Since trade has happened, we update the market price
				currentMarketPrice = bestAsk;
			}
			else
			{
				// LOGIC ERROR SINCE THIS BREAKS ORDER BOOK INVARIANCE
				throw std::logic_error("OrderBook invariance broken in bids.");
			}

			// Reduce the quantity
			quantity -= quantityConsumed;

			// If quantity consumed is zero, there are no more sell orders or no more sell orders
			// at this price level, so we don't exit and go to the next iteration to see if
			// we can match the next price level
		}

		// Set the market price
		m_MarketPrices[order.CompanyID] = currentMarketPrice;
	}

}