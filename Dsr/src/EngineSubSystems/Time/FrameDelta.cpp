#include "dsrpch.h"
#include "FrameDelta.h"

namespace dsr
{
	namespace time
	{
		FrameDelta::FrameDelta()
			: m_previousTime(std::chrono::high_resolution_clock::now())
		{
		}

		void FrameDelta::SetPreviousTime()
		{
			m_previousTime = std::chrono::high_resolution_clock::now();
		}

		Duration FrameDelta::Update()
		{
			using namespace std::chrono;

			high_resolution_clock::time_point currentTime = high_resolution_clock::now();
			duration<float, std::nano> deltaTime = currentTime - m_previousTime;
			m_previousTime = currentTime;

			return Duration(deltaTime);
		}
	}
}
