#include "dsrpch.h"
#include "Time.h"

namespace dsr
{
	namespace time
	{
		void Time::OnPrepareUpdateFrame(const dsr::events::PrepareUpdateFrameEvent& prepareUpdate)
		{
			m_deltaTime = prepareUpdate.GetDeltaTime();
		}
	}
}
