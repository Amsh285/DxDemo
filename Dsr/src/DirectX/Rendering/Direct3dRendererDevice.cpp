#include "dsrpch.h"
#include "Direct3dRendererDevice.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			Direct3dRendererDevice::Direct3dRendererDevice(const std::shared_ptr<Direct3dDevice>& device)
				: m_device(device)
			{
			}

			void Direct3dRendererDevice::UseShaderProgram(const Direct3dShaderProgram& program)
			{
				m_device->SetInputLayout(program.VertexShaderInputLayout.get());
				m_device->UseShader(program.VertexShader.GetShaderPtr().get(), nullptr, 0);

				m_device->UseShader(program.PixelShader.GetShaderPtr().get(), nullptr, 0);
			}
		}
	}
}
