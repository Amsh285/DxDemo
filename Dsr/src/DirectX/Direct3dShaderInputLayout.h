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

			size_t ElementByteStride;
		};

		class Direct3dShaderInputLayout
		{
		public:
			size_t GetTotalStride() const { return m_totalStride; }
			std::vector<Direct3dLayoutShaderInputLayoutDescription> GetLayout() const { return m_layout; }

			void AddVector3f(const std::string& semanticName, const uint32_t& semanticIndex = 0,
				const uint32_t& inputSlot = 0, const uint32_t& alignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
				const D3D11_INPUT_CLASSIFICATION& inputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
				const uint32_t& instanceDataSteprate = 0);
		private:
			std::vector<Direct3dLayoutShaderInputLayoutDescription> m_layout;
			size_t m_totalStride;
		};
	}
}
