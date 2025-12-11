#include <UUIDGenerator.h>
#include <random>

namespace MicroEx
{

	UUIDGenerator* UUIDGenerator::s_Instance = nullptr;

	UUIDGenerator::UUIDGenerator()
	{
	}

	UUIDGenerator::~UUIDGenerator()
	{
	}

	UUIDGenerator& UUIDGenerator::GetInstance()
	{
		if (!s_Instance)
			s_Instance = new UUIDGenerator();

		return *s_Instance;
	}

	uuid_t UUIDGenerator::GenerateUUID()
	{
		static std::random_device rd;
		static std::mt19937_64 gen(rd());
		static std::uniform_int_distribution<uuid_t> dis(0, std::numeric_limits<uuid_t>::max());

		uuid_t newuuid_t;

		do
		{
			newuuid_t = dis(gen);
		} while (!m_GeneratedUUIDs.insert(newuuid_t).second);

		return newuuid_t;
	}

	bool UUIDGenerator::UUIDExists(uuid_t uuid_t) const
	{
		return m_GeneratedUUIDs.find(uuid_t) != m_GeneratedUUIDs.end();
	}

}