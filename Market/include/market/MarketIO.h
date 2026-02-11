#pragma once

#include <core/Core.h>
#include <core/Price.h>
#include <market/MarketConfig.h>
#include <market/CompanyRegistry.h>
#include <Timestamp.h>

namespace MicroEx
{

	struct MICROEX_API InstrumentInitData final
	{
		std::string Name;
		std::string Ticker;
		Timestamp JoinDate;
		price_t OpenPrice;
	};

	struct MICROEX_API MarketSnapshot final
	{
		std::vector<InstrumentInitData> InitData;

		auto begin() noexcept { return InitData.begin(); }
		auto end() noexcept { return InitData.end(); }

		auto begin() const noexcept { return InitData.begin(); }
		auto end() const noexcept { return InitData.end(); }
	};

	class MICROEX_API MarketLoader
	{
	public:
		virtual MarketSnapshot&& Load(const MarketLoadConfig&) const = 0;
		virtual ~MarketLoader() = default;
	};

	class MICROEX_API JSONMarketLoader final : public MarketLoader
	{
	public:
		MarketSnapshot&& Load(const MarketLoadConfig& config) const override;
	};

	class MICROEX_API MarketDumper
	{
	public:
		virtual void Dump(const MarketDumpConfig&, const CompanyRegistry&) const = 0;
		virtual ~MarketDumper() = default;
	};

	class MICROEX_API JSONMarketDumper final : public MarketDumper
	{
	public:
		void Dump(const MarketDumpConfig& config, const CompanyRegistry& registry) const override;
	};

}
