#include "dsrpch.h"
#include "Direct3dSamplerState.h"

namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dSamplerState, dsr_error> Direct3dSamplerState::CreateDefault(std::shared_ptr<Direct3dDevice> device)
		{
			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.MipLODBias = 0.0f;
			desc.MaxAnisotropy = 1;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			desc.BorderColor[0] = 0.0f;
			desc.BorderColor[1] = 0.0f;
			desc.BorderColor[2] = 0.0f;
			desc.BorderColor[3] = 0.0f;
			desc.MinLOD = 0.0f;
			desc.MaxLOD = D3D11_FLOAT32_MAX;

			std::variant<ID3D11SamplerState*, dsr_error> result = device->CreateSamplerState(&desc);
			if (std::holds_alternative<dsr_error>(result))
				return std::get<dsr_error>(result);

			std::shared_ptr<ID3D11SamplerState> stateptr(std::get<ID3D11SamplerState*>(result), [](ID3D11SamplerState* ptr) { SafeRelease(ptr); });
			return Direct3dSamplerState(stateptr);
		}

		Direct3dSamplerState::Direct3dSamplerState()
		{
		}

		Direct3dSamplerState::Direct3dSamplerState(const std::shared_ptr<ID3D11SamplerState>& samplerState)
			: m_samplerState(samplerState)
		{
		}
	}
}
