#pragma once

namespace dsr
{
	namespace directX
	{
		struct Direct3dLayoutShaderInputLayoutDescription
		{
			// don´t use LPCSTR here...
			std::string SemanticName;
			uint32_t SemanticIndex;
			DXGI_FORMAT Format;
			uint32_t InputSlot;
			uint32_t AlignedByteOffset;
			D3D11_INPUT_CLASSIFICATION InputSlotClass;
			uint32_t InstanceDataStepRate;
			uint32_t ElementByteStride;

			Direct3dLayoutShaderInputLayoutDescription()
			{
				SemanticIndex = 0;
				Format = DXGI_FORMAT_UNKNOWN;
				InputSlot = 0;
				AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
				InstanceDataStepRate = 0;
				ElementByteStride = 0;
			}
		};

		class Direct3dShaderInputLayout
		{
		public:
			uint32_t GetTotalStride() const { return m_totalStride; }
			std::vector<Direct3dLayoutShaderInputLayoutDescription> GetLayout() const { return m_layout; }

			Direct3dShaderInputLayout();

			void AddVector3f(const std::string& semanticName, const uint32_t& semanticIndex = 0,
				const uint32_t& inputSlot = 0, const uint32_t& alignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
				const D3D11_INPUT_CLASSIFICATION& inputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
				const uint32_t& instanceDataSteprate = 0);
		private:
			std::vector<Direct3dLayoutShaderInputLayoutDescription> m_layout;
			uint32_t m_totalStride;
		};
	}
}
