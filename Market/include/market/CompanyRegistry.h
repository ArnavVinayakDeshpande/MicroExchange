#pragma once

#include <core/Core.h>
#include <engine/xxHash.h>
#include <Timestamp.h>
#include <string>
#include <chrono>
#include <vector>
#include <optional>
#include <unordered_map>
#include <Logger.h>
#include <UUIDGenerator.h>
#include <core/Price.h>

/**
* Since including files like Windows.h can cause name clashes with var names like ID, Name etc., we undef it
*/

#pragma push_macro("ID")
#pragma push_macro("Name")
#pragma push_macro("Symbol")

#undef ID
#undef Name
#undef Symbol

namespace MicroEx
{

	class MICROEX_API CompanyRegistry;

	struct MICROEX_API CompanyDescriptor final
	{
	public:
		CompanyDescriptor(std::string name, std::string ticker, Timestamp joinDate, price_t referencePrice)
			:
			m_Name(std::move(name)),
			m_Ticker(std::move(ticker)),
			m_JoinDate(std::move(joinDate)),
			m_ReferencePrice(referencePrice)
		{
		}

		~CompanyDescriptor() noexcept = default;

		const std::string& GetName() const
		{
			return m_Name;
		}

		const std::string& GetTicker() const
		{
			return m_Ticker;
		}

		const Timestamp& GetJoinDate() const
		{
			return m_JoinDate;
		}

		company_id_t GetID() const noexcept
		{
			return m_ID;
		}

		price_t GetReferencePrice() const noexcept
		{
			return m_ReferencePrice;
		}

	private:
		std::string m_Name;
		std::string m_Ticker;
		Timestamp m_JoinDate;
		price_t m_ReferencePrice;
		company_id_t m_ID;

	private:
		friend class CompanyRegistry;
	};

	class MICROEX_API CompanyRegistry
	{
	private:
		struct ms_Company
		{
			company_id_t ID;
			std::string Name;
			std::string Ticker;
			Timestamp JoinDate;
			price_t ReferencePrice;
			
			ms_Company()
				:
				ID(0), // invalid ID
				Name(""),
				Ticker(""),
				JoinDate(),
				ReferencePrice(ValueTypes::ZeroPrice)
			{
			}

			ms_Company(company_id_t id, std::string name, std::string ticker, Timestamp joinDate, price_t referencePrice) noexcept
				:
				ID(id),
				Name(std::move(name)),
				Ticker(std::move(ticker)),
				JoinDate(std::move(joinDate)),
				ReferencePrice(referencePrice)
			{
				MICROEX_LOG_CR_INF("Created Company: ID[{0}], Name[{1}], Ticker[{2}]", ID, Name, Ticker);
			}
		};

	public:
		CompanyRegistry();

		~CompanyRegistry();

		uuid_t GetUUID() const noexcept
		{
			return m_UUID;
		}

		std::optional<CompanyDescriptor> GetCompanyByID(company_id_t id) const;
		std::optional<CompanyDescriptor> GetCompanyByName(const std::string& name) const;
		std::optional<CompanyDescriptor> GetCompanyByTicker(const std::string& ticker) const;

		std::optional<std::vector<company_id_t>> GetAllCompanyIDs() const;
		std::optional<std::vector<std::string>> GetAllCompanyNames() const;
		std::optional<std::vector<std::string>> GetAllCompanyTickers() const;
		std::optional<std::vector<CompanyDescriptor>> GetAllCompanies() const;

		size_t GetRegistrySize() const;

		company_id_t AddCompany(const std::string& name, const std::string& ticker, const Timestamp& joinDate, price_t referencePrice);

		void RemoveCompany(company_id_t id);

		bool IsCompanyPresentByID(company_id_t id) const;
		bool IsCompanyPresentByName(const std::string& name) const;
		bool IsCompanyPresentByTicker(const std::string& ticker) const;
		bool Empty() const;

		auto begin() const;
		auto end() const;

		static company_id_t GenerateID(const std::string& name, const std::string& ticker);

		company_id_t operator<<(const CompanyDescriptor& desc);
		void operator>>(company_id_t id);
		std::optional<CompanyDescriptor> operator[](company_id_t id) const;


	private:
		CompanyDescriptor m_GetDescriptor(const ms_Company& company) const;

	private:
		static constexpr XXHASH::xxh64 s_Generator;
		static constexpr std::uint64_t s_IDGeneratorNameSeed = 0xFF124A6BD1890ACE;
		static constexpr std::uint64_t s_IDGeneratorTickerSeed = 0xAC178AFEBD89302F;

	private:
		std::unordered_map<company_id_t, ms_Company> m_Companies;
		std::unordered_map<std::string, company_id_t> m_NameIndex;
		std::unordered_map<std::string, company_id_t> m_TickerIndex;
		uuid_t m_UUID;
	};

}

#pragma pop_macro("ID")
#pragma pop_macro("Name")
#pragma pop_macro("Symbol")