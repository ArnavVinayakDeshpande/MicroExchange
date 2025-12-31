#pragma once

#include <engine/Core.h>
#include <engine/OrderBook.h>
#include <market/CompanyRegistry.h>
#include <map>
#include <optional>

namespace MicroEx
{

	class MICROEX_API MatchEngine
	{
	public:
		MatchEngine(const CompanyRegistry& registry);

		~MatchEngine();

		void PlaceOrder(Order order);

		std::optional<price_t> GetMarketPrice(company_id_t companyID) const;
		std::optional<price_t> operator[](company_id_t companyID) const;
		std::optional<Order> FindOrder(order_id_t orderID) const;
		std::optional<Order >operator()(order_id_t orderID) const;

	private:
		void m_PlaceLimitAsk(Order& order);
		void m_PlaceLimitBid(Order& order);

		void m_PlaceMarketAsk(Order& order);
		void m_PlaceMarketBid(Order& order);

	private:
		OrderBook m_OrderBook;
		std::map<company_id_t, price_t> m_MarketPrices;
	};

}