
#include "dsrpch.h"
#include "Direct3dShaderInputLayout.h"

namespace dsr
{
	namespace directX
	{
		void Direct3dShaderInputLayout::AddVector3f(const std::string& semanticName, const uint32_t& semanticIndex,
			const uint32_t& inputSlot, const uint32_t& alignedByteOffset,
			const D3D11_INPUT_CLASSIFICATION& inputSlotClass, const uint32_t& instanceDataStepRate)
		{
			Direct3dLayoutShaderInputLayoutDescription description;
			description.SemanticName = semanticName;
			description.SemanticIndex = semanticIndex;
			description.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			description.InputSlot = inputSlot;
			description.AlignedByteOffset = alignedByteOffset;
			description.InputSlotClass = inputSlotClass;
			description.InstanceDataStepRate = instanceDataStepRate;

			m_layout.push_back(description);
		}
	}
}
