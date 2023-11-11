#pragma once

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"
#include "DirectX/Direct3dDevice.h"
#include "Direct3dRenderUoW.h"

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

				void AddUnitOfWork(const Direct3dRenderUoW& uow);
				void OnUpdate(const dsr::events::UpdateFrameEvent& updateEvent);
			private:
				std::shared_ptr<Direct3dDevice> m_device;
				std::vector<Direct3dRenderUoW> m_units;
			};
		}
	}
}
