#include "dsrpch.h"
#include "Direct3dRenderer.h"

namespace dsr
{
	namespace directX
	{
		Direct3dRenderer::Direct3dRenderer(const std::shared_ptr<Direct3dDevice>& device)
			: m_device(device)
		{
			assert(device, "device cannot be null");
		}

		void Direct3dRenderer::OnUpdate(const dsr::events::UpdateFrameEvent& updateEvent)
		{
			// not working check later
			m_device->Clear();

			// do 3D rendering on the back buffer here

			// switch the back buffer and the front buffer
			m_device->SwapBuffers();
		}
	}
}
