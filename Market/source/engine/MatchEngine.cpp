#include <engine/MatchEngine.h>
#include <core/Error.h>
#include <UUIDGenerator.h>
#include <Logger.h>

namespace MicroEx
{

	MatchEngine::MatchEngine(const CompanyRegistry& registry) noexcept
		:
		m_Instruments(),
		m_OrderRoutingTable(),
		m_CompanyRegistry(registry),
		m_UUID(UUIDGenerator::GetInstance().GenerateUUID())
	{
		MICROEX_LOG_CR_INF("Creating Market with UUID: {}", m_UUID);

		this->SyncWithRegistry();

		MICROEX_LOG_CR_INF("Creation of Market with UUID: {} completed", m_UUID);
	}

	MatchEngine::~MatchEngine() noexcept
	{
		this->FlushOrderBooks();
		MICROEX_LOG_CR_INF("Deleted Market with UUID: {}", m_UUID);
	}

	void MatchEngine::PlaceOrder(Order order) noexcept
	{
		if (order.Quantity == 0 || order.Price == ValueTypes::ZeroPrice)
		{
			MICROEX_ERR_WRN("Market[{0}] - Invalid Order Placed: OrderID: {1}", m_UUID, order.OrderID);
			return; 
		}

		auto instrIt = m_Instruments.find(order.CompanyID);

		if (instrIt == m_Instruments.end())
		{
			MICROEX_LOG_CR_ERR("Market[{0}] - No Such Company Exists in Registry: {1}", m_UUID, order.CompanyID);
			return;
		}

		auto& instrument = instrIt->second;

		if (order.Type == OrderType::Market)
		{
			if (order.Side == OrderSide::Seller)
				m_PlaceMarketAsk(instrument, order);
			else if (order.Side == OrderSide::Buyer)
				m_PlaceMarketBid(instrument, order);
		}
		else if (order.Type == OrderType::Limit)
		{
			if (order.Side == OrderSide::Seller)
				m_PlaceLimitAsk(instrument, order);
			else if (order.Side == OrderSide::Buyer)
				m_PlaceLimitBid(instrument, order);
		}

		// Add it to routing table
		m_OrderRoutingTable[order.OrderID] = order.CompanyID;

		MICROEX_LOG_CR_INF("Market[{0}] - Placed Order: OrderID: {1}", m_UUID, order.OrderID);
	}

	std::optional<price_t> MatchEngine::GetMarketPrice(company_id_t companyID) const noexcept
	{
		auto it = m_Instruments.find(companyID);

		
		//return it != m_MarketPrices.end() ? it->second : std::nullopt;
	
		if (it == m_Instruments.end())
			return std::nullopt;

		return it->second.MarketPrice;
	}

	std::optional<price_t> MatchEngine::operator[](company_id_t companyID) const noexcept
	{
		return this->GetMarketPrice(companyID);
	}

	std::optional<Order> MatchEngine::FindOrder(order_id_t orderID) const noexcept
	{
		auto routIt = m_OrderRoutingTable.find(orderID);

		if (routIt == m_OrderRoutingTable.end())
			return std::nullopt;

		auto instIt = m_Instruments.find(routIt->second);

		if (instIt == m_Instruments.end())
		{
			MICROEX_ERR_FTL("MatchEngine[{0}] - No company with ID: {1} registered", m_UUID, routIt->second);
			return std::nullopt;
		}

		return instIt->second.Book.FindOrder(orderID);
	}

	std::optional<Order> MatchEngine::operator()(order_id_t orderID) const noexcept
	{
		return this->FindOrder(orderID);
	}

	void MatchEngine::SyncWithRegistry() 
	{
		// Get the company descriptors
		auto desc = m_CompanyRegistry.GetAllCompanies();

		// Iterate through the company registry descriptors to add companies not present
		// in match engine
		for (const auto& c : desc.value())
		{
			if (m_Instruments.find(c.GetID()) != m_Instruments.end())
				continue; // Company exists
			m_Instruments.try_emplace(
				c.GetID(),
				c.GetID(), OrderBook(), c.GetReferencePrice());
		}
	}

	void MatchEngine::FlushOrderBooks() noexcept
	{
		MICROEX_LOG_CR_INF("Market[{0}] - Flushing Order Books", m_UUID);
		
		for (auto it = m_Instruments.begin(); it != m_Instruments.end(); ++it)
		{
			it->second.Book.RemoveAllOrders();
		}
	}

	void MatchEngine::m_PlaceLimitAsk(ms_Instrument& instrument, Order& order) noexcept
	{
		quantity_t& quantity = order.Quantity;
		const price_t& price = order.Price;
		price_t& marketPrice = instrument.MarketPrice;
		OrderBook& book = instrument.Book;

		
		price_t currentMarketPrice = marketPrice;

		while (quantity > 0)
		{
			// Check if we can match
			auto bestBidOpt = book.GetBestBidPrice();

			if (!bestBidOpt)
			{
				MICROEX_LOG_CR_WRN("Market[{0}], OrderBook[{1}] - No Best Bid Available", m_UUID, book.GetUUID());
				break;
			}

			// Best bid exists, compare it to price
			price_t bestBid = bestBidOpt.value();

			if (bestBid < price)
			{

				break;
			}

			// Place the order
			quantity_t quantityConsumed = book.ConsumeBestBid(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best bid
				MICROEX_LOG_CR_INF("Market[{0}], OrderBook[{1}] - Trade Executed at Price: {2} for Quantity: {3}", m_UUID, book.GetUUID(), bestBid.Get(), quantityConsumed);

				// Since trade has happened, we update the market price
				currentMarketPrice = bestBid;
			}
			else
			{
				// THIS MEANS INVARIANCE BROKEN
				MICROEX_ERR_FTL("Market[{0}], OrderBook[{1}] - OrderBook Invariance Broken in Asks", m_UUID, book.GetUUID());
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
			book.InsertAsk(order);

		// Set the market price
		marketPrice = currentMarketPrice;
	}

	void MatchEngine::m_PlaceLimitBid(ms_Instrument& instrument, Order& order) noexcept
	{
		quantity_t& quantity = order.Quantity;
		const price_t& price = order.Price;
		price_t& marketPrice = instrument.MarketPrice;
		OrderBook& book = instrument.Book;

		price_t currentMarketPrice = marketPrice;

		while (quantity > 0)
		{
			// Check if we can match
			auto bestAskOpt = book.GetBestAskPrice();

			if (!bestAskOpt)
			{
				MICROEX_LOG_CR_WRN("Market[{0}], OrderBook[{1}] - No Best Ask Available", m_UUID, book.GetUUID());

				break;
			}

			// Best ask exists, compare it to price
			price_t bestAsk = bestAskOpt.value();

			if (price >= bestAsk)
			{
				break;
			}

			// Place the order
			quantity_t quantityConsumed = book.ConsumeBestAsk(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best ask
				MICROEX_LOG_CR_INF("Market[{0}], OrderBook[{1}] - Trade Executed at Price: {2} for Quantity: {3}", m_UUID, book.GetUUID(), bestAsk.Get(), quantityConsumed);

				// Since trade has happened, we update the market price
				currentMarketPrice = bestAsk;
			}
			else
			{
				// THIS MEANS INVARIANCE BROKEN
				MICROEX_ERR_FTL("Market[{0}], OrderBook[{1}] - OrderBook Invariance Broken in Bids", m_UUID, book.GetUUID());
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
			book.InsertBid(order);

		// Set the market price
		marketPrice = currentMarketPrice;
	}

	void MatchEngine::m_PlaceMarketAsk(ms_Instrument& instrument, Order& order) noexcept
	{
		quantity_t& quantity = order.Quantity;
		price_t& marketPrice = instrument.MarketPrice;
		OrderBook& book = instrument.Book;

		price_t currentMarketPrice = marketPrice;

		while (quantity > 0)
		{
			// Check if we can match
			auto bestBidOpt = book.GetBestBidPrice();

			if (!bestBidOpt)
			{
				MICROEX_LOG_CR_WRN("Market[{0}], OrderBook[{1}] - No Best Bid Available", m_UUID, book.GetUUID());

				break;
			}

			// Best bid exists, compare it to price
			price_t bestBid = bestBidOpt.value();

			// Place the order
			quantity_t quantityConsumed = book.ConsumeBestBid(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best bid

				// Since trade has happened, we update the market price
				currentMarketPrice = bestBid;
			}
			else
			{
				// ORDER BOOK INVARIANCE BROKEN
				MICROEX_ERR_FTL("Market[{0}], OrderBook[{1}] - OrderBook Invariance Broken in Asks", m_UUID, book.GetUUID());
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
		marketPrice = currentMarketPrice;
	}

	void MatchEngine::m_PlaceMarketBid(ms_Instrument& instrument, Order& order) noexcept
	{
		quantity_t& quantity = order.Quantity;
		price_t& marketPrice = instrument.MarketPrice;
		OrderBook& book = instrument.Book;

		price_t currentMarketPrice = marketPrice;

		while (quantity > 0)
		{
			// Check if we can match
			auto bestAskOpt = book.GetBestAskPrice();

			if (!bestAskOpt)
			{
				MICROEX_LOG_CR_WRN("Market[{0}], OrderBook[{1}] - No Best Ask Available", m_UUID, book.GetUUID());

				break;
			}

			// Best ask exists, compare it to price
			price_t bestAsk = bestAskOpt.value();

			// Place the order
			quantity_t quantityConsumed = book.ConsumeBestAsk(quantity);

			// If quantity is zero, we don't add the trade, else we add the trade
			if (quantityConsumed != 0)
			{
				;// Add Trade Callback here; trade happens at best ask


				// Since trade has happened, we update the market price
				currentMarketPrice = bestAsk;
			}
			else
			{
				// ORDER BOOK INVARIANCE BROKEN
				MICROEX_ERR_FTL("Market[{0}], OrderBook[{1}] - OrderBook Invariance Broken in Bids", m_UUID, book.GetUUID());
			}

			// Reduce the quantity
			quantity -= quantityConsumed;

			// If quantity consumed is zero, there are no more sell orders or no more sell orders
			// at this price level, so we don't exit and go to the next iteration to see if
			// we can match the next price level
		}

		// Set the market price
		marketPrice = currentMarketPrice;
	}

}
