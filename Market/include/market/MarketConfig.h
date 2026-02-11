#pragma once

#include <core/Core.h>
#include <core/Error.h>

namespace MicroEx
{

	enum class MICROEX_API MarketFileFormat : std::uint8_t
	{
		JSON,
		Excel
	};

	struct MICROEX_API MarketLoadConfig final
	{
		std::filesystem::path File;
		MarketFileFormat FileFormat;

		MarketLoadConfig() noexcept = delete;

		explicit MarketLoadConfig(const std::filesystem::path& file, MarketFileFormat fileFormat = MarketFileFormat::JSON) noexcept
			:
			File(file),
			FileFormat(fileFormat)
		{
			if (file.empty())
				MICROEX_ERR_FTL("Invalid Market Load Config: Given File Path is Empty");
		}
	};

	enum class MICROEX_API MarketDumpFlags : std::uint32_t
	{
		None = 0,
		DumpPrice = 1 << 0,
		DumpMinMaxPrice = 1 << 1,
		DumpAvgPrice = 1 << 2
	};

	inline MarketDumpFlags MICROEX_API operator&(MarketDumpFlags lhs, MarketDumpFlags rhs) noexcept
	{
		return
			static_cast<MarketDumpFlags>(
				static_cast<std::uint32_t>(lhs) &
				static_cast<std::uint32_t>(rhs));
	}

	inline MarketDumpFlags MICROEX_API operator|(MarketDumpFlags lhs, MarketDumpFlags rhs) noexcept
	{
		return
			static_cast<MarketDumpFlags>(
				static_cast<std::uint32_t>(lhs) |
				static_cast<std::uint32_t>(rhs));
	}

	inline MarketDumpFlags MICROEX_API operator^(MarketDumpFlags lhs, MarketDumpFlags rhs) noexcept
	{
		return
			static_cast<MarketDumpFlags>(
				static_cast<std::uint32_t>(lhs) ^
				static_cast<std::uint32_t>(rhs));
	}

	inline bool MICROEX_API HasFlag(MarketDumpFlags value, MarketDumpFlags flag) noexcept
	{
		return (value & flag) != MarketDumpFlags::None;
	}

	inline MarketDumpFlags MICROEX_API DefaultMarketDumpFlags() noexcept
	{
		return MarketDumpFlags::DumpPrice | MarketDumpFlags::DumpAvgPrice;
	}

	struct MICROEX_API MarketDumpConfig final
	{
		std::filesystem::path File;
		MarketFileFormat FileFormat;
		MarketDumpFlags Flags;

		MarketDumpConfig() noexcept = delete;

		explicit MarketDumpConfig(const std::filesystem::path& file, MarketFileFormat fileFormat = MarketFileFormat::Excel, MarketDumpFlags flags = DefaultMarketDumpFlags()) noexcept
			:
			File(file),
			FileFormat(fileFormat),
			Flags(flags)
		{
			if (file.empty())
				MICROEX_ERR_WRN("Market Dump Config: Empty Filepath given");
		}
	};

	enum class MICROEX_API MarketFlags : std::uint32_t
	{
		None = 0,
		NullPriceInvalid = 1 << 0
	};

	inline MarketFlags MICROEX_API operator&(MarketFlags lhs, MarketFlags rhs) noexcept
	{
		return
			static_cast<MarketFlags>(
				static_cast<std::uint32_t>(lhs) &
				static_cast<std::uint32_t>(rhs));
	}

	inline MarketFlags MICROEX_API operator|(MarketFlags lhs, MarketFlags rhs) noexcept
	{
		return
			static_cast<MarketFlags>(
				static_cast<std::uint32_t>(lhs) |
				static_cast<std::uint32_t>(rhs));
	}

	inline MarketFlags MICROEX_API operator^(MarketFlags lhs, MarketFlags rhs) noexcept
	{
		return
			static_cast<MarketFlags>(
				static_cast<std::uint32_t>(lhs) ^
				static_cast<std::uint32_t>(rhs));
	}

	inline bool MICROEX_API HasFlag(MarketFlags value, MarketFlags flag) noexcept
	{
		return (value & flag) != MarketFlags::None;
	}

	inline MarketFlags MICROEX_API DefaultMarketFlags() noexcept
	{
		return MarketFlags::None;
	}

	struct MICROEX_API MarketConfig final
	{
		MarketLoadConfig LoadConfig;
		MarketDumpConfig DumpConfig;
		MarketFlags Flags;

		MarketConfig() noexcept = delete;

		MarketConfig(MarketLoadConfig loadConfig, MarketDumpConfig dumpConfig, MarketFlags flags = DefaultMarketFlags()) noexcept
			:
			LoadConfig(std::move(loadConfig)),
			DumpConfig(std::move(dumpConfig)),
			Flags(flags)
		{
		}
	};

	enum class MICROEX_API MarketStage : std::uint8_t
	{
		Created,
		Loading,
		Ready,
		Opening,
		Opened,
		Closing,
		Closed,

		Corrupted
	};

}
