#include "dsrpch.h"
#include "RendererSystem.h"

#include "DirectX/Direct3dDeviceShaderExtensions.h"

namespace dsr
{
	namespace ecs
	{
		RendererSystem::RendererSystem(const std::type_index& type, const std::shared_ptr<directX::Direct3dDevice>& device)
			: System(type), m_device(device)
		{
		}

		RendererSystem::RendererSystem(const std::type_index& type, const std::shared_ptr<directX::Direct3dDevice>& device, const int32_t& sortOrder)
			: System(type, sortOrder), m_device(device)
		{
		}

		void RendererSystem::SetupCamera()
		{
			using namespace DirectX;

			using namespace dsr::directX;
			using namespace dsr::scene;

			std::shared_ptr<Camera> camera = Camera::GetActiveCamera();

			XMMATRIX projectionMatrix = DirectX::XMMatrixIdentity();
			XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();
			PixelShaderCameraData psCameraData;

			if (camera)
			{
				std::shared_ptr<ViewProjectionComponent> viewProjection = camera->GetViewProjection();
				std::shared_ptr<TransformComponent> transform = camera->GetTransform();

				projectionMatrix = viewProjection->GetProjectionMatrix();
				viewMatrix = viewProjection->GetViewMatrix();

				DirectX::XMStoreFloat4(&psCameraData.CameraPosition, transform->GetPosition());
			}
			else
			{
				projectionMatrix = XMMatrixIdentity();
				viewMatrix = XMMatrixIdentity();
			}

			SetConstantBuffer(m_device, m_vsConstantBuffers, 0, projectionMatrix);
			SetConstantBuffer(m_device, m_vsConstantBuffers, 1, viewMatrix);

			SetConstantBuffer(m_device, m_psConstantBuffers, 1, &psCameraData, sizeof(PixelShaderCameraData));
		}
	}
}
