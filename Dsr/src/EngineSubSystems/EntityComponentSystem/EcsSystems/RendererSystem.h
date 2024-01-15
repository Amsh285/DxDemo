#pragma once

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dDeviceShaderExtensions.h"
#include "DirectX/Shader/Data/VertexShaderData.h"
#include "DirectX/Textures/Direct3dSamplerState.h"

#include "ErrorHandling/DsrResult.h"

#include "EngineSubSystems/EntityComponentSystem/System.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"

#include "Events/Application/WindowEvents.h"

namespace dsr
{
	namespace ecs
	{
		class RendererSystem : public System
		{
		public:
			virtual std::vector<std::type_index> GetRequiredComponents() const override;
			
			RendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device);

			void PrepareUpdate(const events::PrepareUdateFrameEvent& prepareUpdate);
			void Update(const EngineContext& context);
		private:
			std::shared_ptr<directX::Direct3dDevice> m_device;

			std::map<size_t, directX::Direct3dBuffer> m_vsConstantBuffers;
			std::map<size_t, directX::Direct3dBuffer> m_psConstantBuffers;
		};
	}
}
