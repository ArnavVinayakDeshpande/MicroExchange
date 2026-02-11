#pragma once

#include <core/Core.h>
#include <unordered_set>

namespace MicroEx
{

	class MICROEX_API UUIDGenerator
	{
	public:
		static UUIDGenerator& GetInstance() noexcept;

		~UUIDGenerator() noexcept;

		uuid_t GenerateUUID() noexcept;

		bool UUIDExists(uuid_t uuid_t) const noexcept;

	private:
		UUIDGenerator() noexcept;

	private:
		static UUIDGenerator* s_Instance;

	private:
		std::unordered_set<uuid_t> m_GeneratedUUIDs;
	};

}