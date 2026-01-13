#include <market/CompanyRegistry.h>
#include <engine/xxHash.h>
#include <stdexcept>

namespace MicroEx
{

	CompanyRegistry::CompanyRegistry()
	{
	}

	CompanyRegistry::~CompanyRegistry()
	{
	}

	using CDesc = CompanyDescriptor;

	std::optional<CDesc> CompanyRegistry::GetCompanyByID(company_id_t id) const
	{
		auto it = m_Companies.find(id);

		if (it == m_Companies.end())
			return std::nullopt;

		return m_GetDescriptor(it->second );
	}

	std::optional<CDesc> CompanyRegistry::GetCompanyByName(const std::string& name) const
	{
		auto it = m_NameIndex.find(name);

		if (it == m_NameIndex.end())
			return std::nullopt;

		auto id_it = m_Companies.find(it->second);

		if (id_it == m_Companies.end())
		{
			// Invariance broken
			// TODO Error Handling
			throw std::logic_error("Invariance in Company Registry broken.");
		}

		// TODO Wonder whether I should crosscheck name given with name stored in company in companies

		return m_GetDescriptor(id_it->second);
	}

	std::optional<CDesc> CompanyRegistry::GetCompanyByTicker(const std::string& ticker) const
	{
		auto it = m_TickerIndex.find(ticker);

		if (it == m_TickerIndex.end())
			return std::nullopt;

		auto id_it = m_Companies.find(it->second);

		if (id_it == m_Companies.end())
		{
			// Invariance boroken
			// TODO Error Handling
			throw std::logic_error("Invariance in Company Registry broken.");
		}

		// TODO Wonder whether I should crosscheck ticker

		return m_GetDescriptor(id_it->second);
	}

	std::optional<std::vector<company_id_t>> CompanyRegistry::GetAllCompanyIDs() const
	{
		if (m_Companies.empty())
			return std::nullopt;

		std::vector<company_id_t> ids;
		ids.reserve(m_Companies.size());

		for (const auto& [key, _] : m_Companies)
			ids.emplace_back(key);

		return ids;
	}

	std::optional<std::vector<std::string>> CompanyRegistry::GetAllCompanyNames() const
	{
		if (m_Companies.empty())
			return std::nullopt;

		std::vector<std::string> names;
		names.reserve(m_Companies.size()); // Even though we have name idx, we treate m_Companies as the main and correct registry
	
		for (const auto& [_, company] : m_Companies)
			names.emplace_back(company.Name);

		return names;
	}

	std::optional<std::vector<std::string>> CompanyRegistry::GetAllCompanyTickers() const
	{
		if (m_Companies.empty())
			return std::nullopt;

		std::vector<std::string> tickers;
		tickers.reserve(m_Companies.size()); // EVen though we have ticker idx, we treate m_Companies as the main and correct registry

		for (const auto& [_, company] : m_Companies)
			tickers.emplace_back(company.Ticker);

		return tickers;
	}

	std::optional<std::vector<CDesc>> CompanyRegistry::GetAllCompanies() const
	{
		if (m_Companies.empty())
			return std::nullopt;

		std::vector<CDesc> companies;
		companies.reserve(m_Companies.size());

		for (auto it = m_Companies.begin(); it != m_Companies.end(); ++it)
			companies.emplace_back(m_GetDescriptor(it->second));

		return companies;
	}

	size_t CompanyRegistry::GetRegistrySize() const
	{
		return m_Companies.size();
	}

	company_id_t CompanyRegistry::AddCompany(const std::string& name, const std::string& ticker, const Timestamp& joinDate)
	{
		// TODO Name UNiquness -> Ticker Uniqueness -> ID Generation
	
		if (name.empty() || ticker.empty())
		{
			return 0; // TODO: return invalid here, returning 0 for now
		}

		if (this->IsCompanyPresentByName(name))
		{
			return 0; // TODO: return invalid here + log, returning 0 for now
		}

		if (this->IsCompanyPresentByTicker(ticker))
		{
			return 0; // TODO: return invalid here + log, returning 0 for now
		}

		company_id_t idGenerated = CompanyRegistry::GenerateID(name, ticker);

		// Check for invalid id here

		// Update company registry
		m_Companies[idGenerated] = ms_Company(idGenerated, name, ticker, joinDate);

		// Update name index
		m_NameIndex[name] = idGenerated;

		// Update ticker index
		m_TickerIndex[ticker] = idGenerated;

		return idGenerated;
	}

	void CompanyRegistry::RemoveCompany(company_id_t id)
	{
		// Check if id is invalid here

		// Check if id exists
		auto it = m_Companies.find(id);

		if (it == m_Companies.end())
			return;

		// Company does exist, remove it
		// Remove it from name index
		m_NameIndex.erase(it->second.Name);
		
		// Remove it from ticker index
		m_TickerIndex.erase(it->second.Ticker);

		// Remove it from registry
		m_Companies.erase(it);
	}

	bool CompanyRegistry::IsCompanyPresentByID(company_id_t id) const
	{
		return m_Companies.find(id) != m_Companies.end();
	}

	bool CompanyRegistry::IsCompanyPresentByName(const std::string& name) const
	{
		return m_NameIndex.find(name) != m_NameIndex.end();
	}

	bool CompanyRegistry::IsCompanyPresentByTicker(const std::string& ticker) const
	{
		return m_TickerIndex.find(ticker) != m_TickerIndex.end();
	}

	bool CompanyRegistry::Empty() const
	{
		return m_Companies.empty();
	}

	auto CompanyRegistry::begin() const
	{
		return m_Companies.begin();
	}

	auto CompanyRegistry::end() const
	{
		return m_Companies.end();
	}

	company_id_t CompanyRegistry::GenerateID(const std::string& name, const std::string& ticker)
	{
		if (name.empty() || ticker.empty())
			return 0; // TODO RETURN INVALID ID AND LOG ERROR, FOR NOW RETURN 0

		std::string buffer = name + " | " + ticker;

		// TODO CHECK HOW TO STRUCTURE THE SEEDS
		std::uint64_t idGenerated =
			s_Generator.hash(
				buffer.c_str(),
				buffer.size(),
				s_IDGeneratorNameSeed ^ s_IDGeneratorTickerSeed);

		company_id_t finalID = static_cast<company_id_t>(idGenerated);

		return finalID;
	}

	company_id_t CompanyRegistry::operator<<(const CompanyDescriptor& desc)
	{
		return this->AddCompany(desc.m_Name, desc.m_Ticker, desc.m_JoinDate);
	}

	void CompanyRegistry::operator>>(company_id_t id)
	{
		return this->RemoveCompany(id);
	}

	std::optional<CompanyDescriptor> CompanyRegistry::operator[](company_id_t id) const
	{
		return this->GetCompanyByID(id);
	}

	CompanyDescriptor CompanyRegistry::m_GetDescriptor(const ms_Company& company) const
	{
		CompanyDescriptor descriptor(company.Name, company.Ticker, company.JoinDate);
		descriptor.m_ID = company.ID;

		return descriptor;
	}

}