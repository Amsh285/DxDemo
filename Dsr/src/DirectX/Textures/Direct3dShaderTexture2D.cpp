#include "dsrpch.h"
#include "Direct3dShaderTexture2D.h"

namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dShaderTexture2D, dsr_error> Direct3dShaderTexture2D::LoadSingleRGBA(
			const std::shared_ptr<Direct3dDevice> device,
			const std::filesystem::path& fileName,
			const uint32_t& mipLevels,
			const uint32_t& miscFlags,
			const DXGI_SAMPLE_DESC& sampleDesc)
		{
			dsr::vendor::STBIImage image(fileName);

			if (!image)
			{
				std::string errorMessage = "Error loading Image: ";
				errorMessage += fileName.string();
				return dsr_error(errorMessage, ERROR_LOADING_IMAGE);
			}

			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
			textureDesc.Width = image.GetWidth();
			textureDesc.Height = image.GetHeight();
			textureDesc.MipLevels = mipLevels;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc = sampleDesc;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = miscFlags;

			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
			initData.pSysMem = image.GetImagePtr();
			initData.SysMemPitch = image.GetWidth() * 4;

			std::variant<ID3D11Texture2D*, dsr_error> createTexture2DResult = device->CreateTexture2D(&textureDesc, &initData);
			if (std::holds_alternative<dsr_error>(createTexture2DResult))
				return std::get<dsr_error>(createTexture2DResult);

			std::shared_ptr<ID3D11Texture2D> texturePtr(std::get<ID3D11Texture2D*>(createTexture2DResult), [](ID3D11Texture2D* ptr) {SafeRelease(ptr);});

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			//srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2D.MipLevels = -1;

			std::variant<ID3D11ShaderResourceView*, dsr_error> createSRVResult = device->CreateShaderResourceView(texturePtr.get(), &srvDesc);
			if (std::holds_alternative<dsr_error>(createSRVResult))
				return std::get<dsr_error>(createSRVResult);

			std::shared_ptr<ID3D11ShaderResourceView> srvPtr(std::get<ID3D11ShaderResourceView*>(createSRVResult), [](ID3D11ShaderResourceView* ptr) {SafeRelease(ptr);});

			return Direct3dShaderTexture2D(texturePtr, srvPtr);
		}

		Direct3dShaderTexture2D::Direct3dShaderTexture2D(
			const std::shared_ptr<ID3D11Texture2D>& texture,
			const std::shared_ptr<ID3D11ShaderResourceView>& shaderResourceView)
			: m_texture(texture), m_shaderResourceView(shaderResourceView)
		{
		}
	}
}
