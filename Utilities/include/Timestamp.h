#pragma once

#include <engine/Core.h>
#include <chrono>

namespace MicroEx
{

	class MICROEX_API Timestamp
	{
	public:
		using Clock = std::chrono::system_clock;
		using Timepoint = Clock::time_point;

	public:
		Timestamp()
			:
			m_Timepoint(this->Now())
		{
		}

		Timestamp(Timepoint timepoint)
			:
			m_Timepoint(std::move(timepoint))
		{
		}

		operator Timepoint()
		{
			return m_Timepoint;
		}

		static Timepoint Now()
		{
			return Clock::now();
		}

	private:
		Timepoint m_Timepoint;
	};

}