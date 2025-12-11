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

		uuid_t GenerateUUID();

		bool UUIDExists(uuid_t uuid_t) const;

	private:
		UUIDGenerator();

	private:
		static UUIDGenerator* s_Instance;

	private:
		std::unordered_set<uuid_t> m_GeneratedUUIDs;
	};

}