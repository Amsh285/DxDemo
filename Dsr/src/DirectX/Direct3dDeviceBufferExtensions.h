#pragma once

#include "Direct3dDevice.h"
#include "Direct3dVertexBufferf.h"
#include "Shader/Direct3dShaderInputLayout.h"
#include "Buffers/Direct3dBuffer.h"

namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dVertexBufferf, dsr_error> SetupVertexBufferf(
			const std::shared_ptr<Direct3dDevice> device,
			const std::vector<float>& vertexData,
			const std::vector<uint32_t>& indexBuffer,
			const Direct3dShaderInputLayout& shaderInputLayout
		);
	}
}
