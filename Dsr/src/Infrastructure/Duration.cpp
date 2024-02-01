#include "dsrpch.h"
#include "Duration.h"

namespace dsr
{
	Duration::Duration()
		: m_duration(0.0f)
	{
	}

	Duration::Duration(const std::chrono::duration<float, std::nano>& duration)
		: m_duration(duration)
	{
	}

	Duration Duration::Capped(const std::chrono::duration<float, std::nano>& cap) const
	{
		return Duration(std::min(m_duration, cap));
	}
}
