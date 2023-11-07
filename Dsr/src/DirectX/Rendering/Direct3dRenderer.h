#pragma once

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"
#include "DirectX/Direct3dDevice.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			class Direct3dRenderer : public dsr::events::EventListener
			{
			public:
				Direct3dRenderer(const std::shared_ptr<Direct3dDevice>& device);

				void OnUpdate(const dsr::events::UpdateFrameEvent& updateEvent);
			private:
				std::shared_ptr<Direct3dDevice> m_device;
			};
		}
	}
}
