#include <market/CompanyRegistry.h>
#include <UUIDGenerator.h>
#include <stdexcept>

namespace MicroEx
{

	std::vector<uint8_t> SerializeData(const Company& company)
	{
		std::vector<uint8_t> data;

		// Serialize Name
		{
			// Add length of name
			uint32_t length = company.Name.size();
			uint8_t* lengthPtr = reinterpret_cast<uint8_t*>(&length);
		
			data.insert(data.end(), lengthPtr, lengthPtr + sizeof(uint32_t));
			data.insert(data.end(), company.Name.begin(), company.Name.end());
		}

		// Serialize Symbol
		{
			// Add length of symbol
			uint32_t length = company.Symbol.size();
			uint8_t* lengthPtr = reinterpret_cast<uint8_t*>(&length);

			data.insert(data.end(), lengthPtr, lengthPtr + sizeof(uint32_t));
			data.insert(data.end(), company.Symbol.begin(), company.Symbol.end());
		}

		// Serialize company_id_t
		{
			const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&company.ID);

			data.insert(data.end(), ptr, ptr + sizeof(company_id_t));
		}

		// Serialize Join Date
		{
			int64_t value = 
				std::chrono::duration_cast<std::chrono::microseconds>(
					company.JoinDate.time_since_epoch()).count();

			uint8_t* ptr = reinterpret_cast<uint8_t*>(&value);

			data.insert(data.end(), ptr, ptr + sizeof(int64_t));
		}

		return data;
	}

	std::optional<Company> DeserializeData(const std::vector<uint8_t>& data)
	{
		if (data.empty())
			return std::nullopt;

		//static_assert(false, "TODO");
	}

	CompanyRegistry::CompanyRegistry()
	{
	}

	CompanyRegistry::~CompanyRegistry()
	{
	}

	std::optional<Company> CompanyRegistry::GetCompanyByID(company_id_t id) const
	{
		auto it = m_Companies.find(id);

		if (it == m_Companies.end())
			return std::nullopt;

		return it->second;
	}

	std::optional<Company> CompanyRegistry::GetCompanyByName(const std::string& name) const
	{
		for (const auto& company : m_Companies)
		{
			if (name == company.second.Name)
				return company.second;
		}

		return std::nullopt;
	}

	std::optional<Company> CompanyRegistry::GetCompanyBySymbol(const std::string& symbol) const
	{
		for (const auto& company : m_Companies)
		{
			if (symbol == company.second.Symbol)
				return company.second;
		}

		return std::nullopt;
	}

	std::optional<company_id_t> CompanyRegistry::AddCompany(const std::string& name, const std::string& symbol)
	{
		if (name.empty() || symbol.empty())
			return std::nullopt;

		// Check if company with name or symbol already exists
		// TODO Change exceptions to error handling later on, add an error queue since we 
		// will be returning optionals we can add this to error queue.
		for (const auto& company : m_Companies)
		{
			const auto& c = company.second;
			 
			if (c.Name == name)
				throw std::invalid_argument("Company with given name already exists with Stock ID: " + std::to_string(c.ID));
		
			if (c.Symbol == symbol)
				throw std::invalid_argument("Company with given ticker symbol already exists with Stock ID: " + std::to_string(c.ID));
		}

		// Get a new stock id
		company_id_t id = UUIDGenerator::GetInstance().GenerateUUID();
		
		// Since IDs are guaranteed to be universally unique under the current implementation
		// we can directly add it using the [] operator
		m_Companies[id] = Company(id, name, symbol, std::chrono::system_clock::now());
	
		return id;
	}

	bool CompanyRegistry::RemoveCompanyByID(company_id_t id)
	{
		bool removed = static_cast<bool>(m_Companies.erase(id));

		// Add later to log queues if existed or not, no need to add this to error queue
		return removed;
	}

	bool CompanyRegistry::RemoveCompanyByName(const std::string& name)
	{
		auto opt = this->GetCompanyByName(name);

		if (!opt.has_value())
			return false; // error queue

		return this->RemoveCompanyByID(opt->ID);
	}

	bool CompanyRegistry::RemoveCompanyBySymbol(const std::string& symbol)
	{
		auto opt = this->GetCompanyBySymbol(symbol);

		if (!opt.has_value())
			return false; // error queue

		return this->RemoveCompanyByID(opt->ID);
	}

	bool CompanyRegistry::IsCompanyPresentByID(company_id_t id) const
	{
		return m_Companies.find(id) != m_Companies.end();
	}

	bool CompanyRegistry::IsCompanyPresentByName(const std::string& name) const
	{
		for (const auto& company : m_Companies)
		{
			if (name == company.second.Name)
				return true;
		}

		return false;
	}

	bool CompanyRegistry::IsCompanyPresentBySymbol(const std::string& symbol) const
	{
		for (const auto& company : m_Companies)
		{
			if (symbol == company.second.Symbol)
				return true;
		}

		return false;
	}

}