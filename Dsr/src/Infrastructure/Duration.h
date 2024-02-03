#pragma once

namespace dsr
{
	class Duration
	{
	public:
		float Seconds() const { return Sec().count(); }
		float MilliSeconds() const { return Milli().count(); }
		float MicroSeconds() const { return Micro().count(); }
		float NanoSeconds() const { return Nano().count(); }

		std::chrono::duration<float> Sec() const { return m_duration; }
		std::chrono::duration<float, std::milli> Milli() const { return m_duration; }
		std::chrono::duration<float, std::micro> Micro() const { return m_duration; }
		std::chrono::duration<float, std::nano> Nano() const { return m_duration; }

		Duration();
		explicit Duration(const std::chrono::duration<float, std::nano>& duration);

		Duration Capped(const std::chrono::duration<float, std::nano>& cap) const;
	private:
		std::chrono::duration<float, std::nano> m_duration;
	};
}
