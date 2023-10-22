#include "dsrpch.h"
#include "Direct3dRendererDevice.h"

namespace dsr
{
	namespace directX
	{
		Direct3dRendererDevice::Direct3dRendererDevice(const std::shared_ptr<Direct3dDevice>& device)
			: m_device(device)
		{
		}

		void Direct3dRendererDevice::SetShaderProgram(const Direct3dShaderProgram& program)
		{

		}


	}
}
