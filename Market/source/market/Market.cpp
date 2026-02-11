#include <market/Market.h>
#include <market/marketLoader.h>

namespace MicroEx
{	

	Market::Market(MarketConfig config) noexcept
		:
		m_UUID(UUIDGenerator::GetInstance().GenerateUUID()),
		m_Config(std::move(config)),
		m_CurrentStage(MarketStage::Created),
		m_CompanyRegistry(),
		m_MatchEngine(nullptr),
		m_Initialized(false)
	{
		MICROEX_LOG_CR_INF("New Market created with UUID: {0}", m_UUID);
	}

	Market::~Market() noexcept
	{
		MICROEX_LOG_CR_INF("Market with UUID: {0} destroyed", m_UUID);
	}

	void Market::Initialize() noexcept
	{
		// Validate config
		if (!m_ValidateConfig())
		{
			MICROEX_LOG_CR_CRT("Market[{0}] - Invalid Configuration; Transitioning to Corrupted State", m_UUID);
			m_TransitionTo(MarketStage::Corrupted);
			return;
		}
		m_TransitionTo(MarketStage::Loading);

		// Create the Company Registry
		m_CreateCompanyRegistry();

		// Create the Matching Engine
		m_MatchEngine = std::make_unique<MatchEngine>(m_CompanyRegistry);

		if (!m_MatchEngine)
		{
			MICROEX_LOG_CR_CRT("Market[{0}] - Failed to create Match Engine; Transitioning to Corrupted State", m_UUID);
			m_TransitionTo(MarketStage::Corrupted);
		}

		MICROEX_LOG_CR_INF("Market[{0}] - Succesfully Initialized", m_UUID);
	}

	bool Market::m_ValidateConfig() noexcept
	{
		// Validate load config
		if (m_Config.LoadConfig.File.empty())
		{
			MICROEX_LOG_CR_ERR("Market[{0}] - Load Config File Path is Empty", m_UUID);
			return false;
		}

		switch (m_Config.LoadConfig.FileFormat)
		{
			case MarketFileFormat::JSON:
				if (m_Config.LoadConfig.File.extension() != ".json")
				{
					MICROEX_LOG_CR_ERR("Market[{0}] - Load Config File Extension does not match specified File Format", m_UUID);
					return false;
				}
				break;

			case MarketFileFormat::Excel:
				if (m_Config.LoadConfig.File.extension() != ".xlsx" && m_Config.LoadConfig.File.extension() != ".xls")
				{
					MICROEX_LOG_CR_ERR("Market[{0}] - Load Config File Extension does not match specified File Format", m_UUID);
					return false;
				}
				break;
		}

		return true;
	}

	void Market::m_CreateCompanyRegistry() noexcept
	{
		std::unique_ptr<MarketLoader> loader;

		switch (m_Config.LoadConfig.FileFormat)
		{
		case MarketFileFormat::JSON:
			loader = std::make_unique<JSONMarketLoader>();
			break;

		case MarketFileFormat::Excel:
			break;
		}

		if (!loader)
		{
			MICROEX_ERR_FTL("Market[{0}] - Could not create company registry.", m_UUID);
		}

		MarketSnapshot snapshot = loader->Load(m_Config.LoadConfig.File);
	
		for (const auto& instr : snapshot)
		{
			m_CompanyRegistry.AddCompany(instr.Name, instr.Ticker, instr.JoinDate, instr.OpenPrice);
		}
	}

	void Market::m_TransitionTo(MarketStage next) noexcept
	{
		if (m_IsValidStageTransition(m_CurrentStage, next))
		{
			MICROEX_LOG_CR_INF("Market[{0}] - Transitioning from Stage: {1} to Stage: {2}", m_UUID, static_cast<std::uint8_t>(m_CurrentStage), static_cast<std::uint8_t>(next));
			m_CurrentStage = next;
		}
		else
		{
			MICROEX_ERR_FTL("Market[{0}] - Invalid Stage Transition from Stage: {1} to Stage: {2}", m_UUID, static_cast<std::uint8_t>(m_CurrentStage), static_cast<std::uint8_t>(next));
		}
	}

	constexpr bool Market::m_IsValidStageTransition(MarketStage from, MarketStage to) noexcept
	{
		switch (from)
		{
		case MarketStage::Corrupted:
			return false;

		case MarketStage::Created:
			return to == MarketStage::Loading || to == MarketStage::Corrupted;

		case MarketStage::Loading:
			return to == MarketStage::Ready || to == MarketStage::Corrupted;

		case MarketStage::Ready:
			return to == MarketStage::Opening || to == MarketStage::Corrupted;

		case MarketStage::Opening:
			return to == MarketStage::Opened || to == MarketStage::Corrupted;

		case MarketStage::Opened:
			return to == MarketStage::Closing || to == MarketStage::Corrupted;

		case MarketStage::Closing:
			return to == MarketStage::Closed || to == MarketStage::Corrupted;

		case MarketStage::Closed:
			return to == MarketStage::Opening || to == MarketStage::Corrupted;
		}
	}

}
