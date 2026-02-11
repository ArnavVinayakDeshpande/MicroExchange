#pragma once

#include <core/Core.h>
#include <market/MarketConfig.h>
#include <engine/MatchEngine.h>
#include <market/CompanyRegistry.h>
#include <memory>
#include <core/Error.h>
#include <UUIDGenerator.h>

namespace MicroEx
{

	class MICROEX_API Market final
	{
	public:
		Market() noexcept = delete;

		explicit Market(MarketConfig config) noexcept;

		~Market() noexcept;

		constexpr uuid_t GetUUID() const noexcept
		{
			return m_UUID;
		}

		constexpr const MarketConfig& GetConfig() const noexcept
		{
			return m_Config;
		}

		constexpr MarketStage GetCurrentStage() const noexcept
		{
			return m_CurrentStage;
		}

		constexpr const CompanyRegistry& GetCompanyRegistry() const noexcept
		{
			return m_CompanyRegistry;
		}

		void Initialize() noexcept;
		void Terminate() noexcept;

		void Open() noexcept;
		void Close() noexcept;

	private:
		bool m_ValidateConfig() noexcept;
		void m_CreateCompanyRegistry() noexcept;
		void m_TransitionTo(MarketStage next) noexcept;
		constexpr bool m_IsValidStageTransition(MarketStage from, MarketStage to) noexcept;

	private:
		uuid_t m_UUID;
		MarketConfig m_Config;
		MarketStage m_CurrentStage;
		CompanyRegistry m_CompanyRegistry;
		std::unique_ptr<MatchEngine> m_MatchEngine;
		bool m_Initialized;
	};


} // namespace MicroEx
