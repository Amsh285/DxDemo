#pragma once

#include "DirectX/Direct3dDevice.h"
#include "Vendor/wrappers/STBIImage.h"

namespace dsr
{
	namespace directX
	{
		constexpr auto ERROR_LOADING_IMAGE = 101;
		constexpr auto ERROR_LOADING_IMAGE_INVALIDARGUMENT = 102;

		class Direct3dShaderTexture2D
		{
		public:
			static std::variant<Direct3dShaderTexture2D, dsr_error> LoadSingleRGBA(
				const std::shared_ptr<Direct3dDevice> device,
				const std::filesystem::path& fileName,
				const uint32_t& mipLevels = 1,
				const uint32_t& miscFlags = 0,
				const DXGI_SAMPLE_DESC & sampleDesc = { 1, 0 });

			std::shared_ptr<ID3D11Texture2D> GetTexturePtr() const { return m_texture; }
			std::shared_ptr<ID3D11ShaderResourceView> GetShaderResourceViewPtr() const { return m_shaderResourceView; }

			Direct3dShaderTexture2D(
				const std::shared_ptr<ID3D11Texture2D>& texture,
				const std::shared_ptr<ID3D11ShaderResourceView>& shaderResourceView);
		private:
			std::shared_ptr<ID3D11Texture2D> m_texture;
			std::shared_ptr<ID3D11ShaderResourceView> m_shaderResourceView;
		};
	}
}
