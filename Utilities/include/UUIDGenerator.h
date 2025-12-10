#pragma once

#include <engine/Core.h>
#include <unordered_set>

namespace MicroEx
{

	class MICROEX_API UUIDGenerator
	{
	public:
		static UUIDGenerator& GetInstance();

		~UUIDGenerator();

		UUID GenerateUUID();

		bool UUIDExists(UUID uuid) const;

	private:
		UUIDGenerator();

	private:
		static UUIDGenerator* s_Instance;

	private:
		std::unordered_set<UUID> m_GeneratedUUIDs;
	};

}