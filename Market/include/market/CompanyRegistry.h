#pragma once

#include <engine/Core.h>
#include <string>
#include <chrono>
#include <vector>
#include <optional>
#include <unordered_map>

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

	struct MICROEX_API Company
	{
	public:
		company_id_t ID;
		std::string Name;
		std::string Symbol;
		std::chrono::system_clock::time_point JoinDate;

	public:
		Company()
			:
			ID(0),
			Name(""),
			Symbol(""),
			JoinDate(std::chrono::system_clock::now())
		{
		}

		Company(company_id_t id, const std::string& name, const std::string& symbol, std::chrono::system_clock::time_point joinDate)
			:
			ID(id),
			Name(name),
			Symbol(symbol),
			JoinDate(joinDate)
		{
		}

		Company(company_id_t id, const std::string& name, const std::string& symbol)
			:
			ID(id),
			Name(name),
			Symbol(symbol),
			JoinDate(std::chrono::system_clock::now())
		{
		}
	};

	std::vector<uint8_t> SerializeData(const Company& company);
	std::optional<Company> DeserializeData(const std::vector<uint8_t>& data);

	class MICROEX_API CompanyRegistry
	{
	public:
		CompanyRegistry();

		~CompanyRegistry();

		std::optional<Company> GetCompanyByID(company_id_t id) const;
		std::optional<Company> GetCompanyByName(const std::string& name) const;
		std::optional<Company> GetCompanyBySymbol(const std::string& symbol) const;

		std::optional<company_id_t> AddCompany(const std::string& name, const std::string& symbol);
		bool RemoveCompanyByID(company_id_t id);
		bool RemoveCompanyByName(const std::string& name);
		bool RemoveCompanyBySymbol(const std::string& symbol);

		bool IsCompanyPresentByID(company_id_t id) const;
		bool IsCompanyPresentByName(const std::string& name) const;
		bool IsCompanyPresentBySymbol(const std::string& symbol) const;

	private:
		std::unordered_map<company_id_t, Company> m_Companies;
	};

}

#pragma pop_macro("ID")
#pragma pop_macro("Name")
#pragma pop_macro("Symbol")