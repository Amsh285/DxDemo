#pragma once

#include "Direct3dDevice.h"
#include "Direct3dVertexBufferf.h"
#include "Direct3dShaderInputLayout.h"

namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dVertexBufferf, dsr_error> LoadVertexBufferf(
			const std::shared_ptr<Direct3dDevice> device,
			const std::vector<float>& vertexData,
			const std::vector<uint32_t>& indexBuffer,
			const Direct3dShaderInputLayout& shaderInputLayout
		);
	}
}
