#pragma once

#include "DirectX/Direct3dDevice.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dSamplerState
		{
		public:
			static std::variant<Direct3dSamplerState, dsr_error> CreateDefault(std::shared_ptr<Direct3dDevice> device);

			std::shared_ptr<ID3D11SamplerState> GetSamplerStatePtr() const { return m_samplerState; };

			Direct3dSamplerState();
			Direct3dSamplerState(const std::shared_ptr<ID3D11SamplerState>& samplerState);
		private:
			std::shared_ptr<ID3D11SamplerState> m_samplerState;
		};
	}
}
