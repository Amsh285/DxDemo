#pragma once

#include "CameraSystem/Camera.h"

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dDeviceShaderExtensions.h"

#include "Direct3dRenderUoW.h"

#include "DirectX/Shader/Data/VertexShaderData.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			class Direct3dRenderer : public dsr::events::EventListener
			{
			public:
				std::shared_ptr<camerasystem::Camera> GetActiveCamera() const { return m_activeCamera; }
				void SetActiveCamera(const std::shared_ptr<camerasystem::Camera>& camera) { m_activeCamera = camera; }

				Direct3dRenderer(const std::shared_ptr<Direct3dDevice>& device);

				void AddUnitOfWork(const Direct3dRenderUoW& uow);
				void OnUpdate(const dsr::events::UpdateFrameEvent& updateEvent);
			private:
				DsrResult SetVertexShaderPerObjectData(
					const std::shared_ptr<Direct3dDevice> device,
					std::shared_ptr<Direct3dShader<ID3D11VertexShader>> target,
					const size_t& bRegister,
					const dsr::shader::PerObject& data);


				std::shared_ptr<Direct3dDevice> m_device;
				std::vector<Direct3dRenderUoW> m_units;

				std::shared_ptr<camerasystem::Camera> m_activeCamera;
			};
		}
	}
}
