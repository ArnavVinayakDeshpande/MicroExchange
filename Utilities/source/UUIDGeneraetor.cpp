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

	UUID UUIDGenerator::GenerateUUID()
	{
		static std::random_device rd;
		static std::mt19937_64 gen(rd());
		static std::uniform_int_distribution<UUID> dis(0, std::numeric_limits<UUID>::max());

		UUID newUUID;

		do
		{
			newUUID = dis(gen);
		} while (!m_GeneratedUUIDs.insert(newUUID).second);

		return newUUID;
	}

	bool UUIDGenerator::UUIDExists(UUID uuid) const
	{
		return m_GeneratedUUIDs.find(uuid) != m_GeneratedUUIDs.end();
	}

}