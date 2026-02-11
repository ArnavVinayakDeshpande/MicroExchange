#include <Market/MarketIO.h>
#include <nholmann/json.h>
#include <core/Error.h>
#include <functional>

namespace MicroEx
{

	using json = nlohmann::json;
	using JsonEmitter = std::function<void(json&, const CompanyDescriptor&)>;

	static std::vector<JsonEmitter> BuildDumpEmitter(MarketDumpFlags flags)
	{
		std::vector<JsonEmitter> emitter;

		if (HasFlag(flags, MarketDumpFlags::DumpPrice))
		{
			static_assert(false);
		}
	}

	MarketSnapshot&& JSONMarketLoader::Load(const MarketLoadConfig& config) const
	{
		if (config.FileFormat != MarketFileFormat::JSON)
		{
			MICROEX_ERR_FTL("Using wrong market loader for given file type {0}", config.File.extension());
		}

		std::ifstream ifstr(config.File);

		if (!ifstr.is_open())
		{
			MICROEX_ERR_FTL("Load File could not be opened.");
		}

		json root;

		ifstr >> root;

		ifstr.close();

		if (!root.is_array())
		{
			MICROEX_ERR_FTL("Load File does not have array")
		}

		MarketSnapshot snapshot;

		for (const auto& entry : root)
		{
			InstrumentInitData data;

			if (!entry.is_object())
			{
				MICROEX_ERR_FTL("Load file entry is not an object.")
			}

			if (!entry.contains("name") || !entry["name"].is_string())
			{
				// error
			}
			data.Name = entry["name"].get<std::string>();

			if (!entry.contains("ticker") || !entry["ticker"].is_string())
			{
				// error
			}
			data.Ticker = entry["ticker"].get<std::string>();

			if (!entry.contains("join_date") || !entry["join_date"].is_string())
			{
				// error
			}
			// to join date
			data.JoinDate = Timestamp();

			if (!entry.contains("open_price") || !entry["open_price"].is_number())
			{
				// error
			}
			data.OpenPrice = price_t(entry["open_price"].get<double>());

			snapshot.InitData.emplace_back(std::move(data));
		}

		return std::move(snapshot);
	}

	void JSONMarketDumper::Dump(const MarketDumpConfig& config, const CompanyRegistry& registry) const
	{
		if (config.FileFormat != MarketFileFormat::JSON)
		{
			MICROEX_ERR_FTL("Using wrong market dumper for given file type: {0}", config.File.extension())
		}

		std::ofstream ofstr(config.File);

		if (!ofstr.is_open())
		{
			MICROEX_ERR_FTL("Dump File could not be opened.");
		}

		json arr = json::array();

		std::function<void(json& obj)> addon;
		
		ofstr << arr.dump(2);

		ofstr.close();
	}

}
