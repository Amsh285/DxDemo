#pragma once

#include "Direct3dDevice.h"
#include "Direct3dShaderProgram.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dRendererDevice
		{
		public:
			std::shared_ptr<Direct3dDevice> GetDevice() const { return m_device; }
			
			Direct3dRendererDevice(const std::shared_ptr<Direct3dDevice>& device);

			void UseShaderProgram(const Direct3dShaderProgram& program);
		private:
			std::shared_ptr<Direct3dDevice> m_device;
		};
	}
}
