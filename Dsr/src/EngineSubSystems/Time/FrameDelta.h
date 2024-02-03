#pragma once

#include "Infrastructure/Duration.h"

namespace dsr
{
	namespace time
	{
		class FrameDelta
		{
		public:
			FrameDelta();

			void SetPreviousTime();
			Duration Update();
		private:
			std::chrono::high_resolution_clock::time_point m_previousTime;
		};
	}
}
