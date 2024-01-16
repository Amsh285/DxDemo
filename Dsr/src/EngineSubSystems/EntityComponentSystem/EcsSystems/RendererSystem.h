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
		// funktioniert wenn man nur von Eventlistener erbt
		// public dsr::events::EventListener
		// Andere vererbungen f�hren zu probleme. Muss mir das genauer ansehen T_T
		class RendererSystem : public System
		{
		public:
			virtual std::vector<std::type_index> GetRequiredComponents() const override;
			
			RendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device);
			RendererSystem(const RendererSystem& other) = delete;
			RendererSystem& operator=(const RendererSystem& other) = delete;

			void PrepareUpdate(const events::PrepareUdateFrameEvent& args);
			void Update(const EngineContext& context);
			void UpdateFinished(const events::UpdateFrameFinishedEvent& args);
		private:
			std::shared_ptr<directX::Direct3dDevice> m_device;

			std::map<size_t, directX::Direct3dBuffer> m_vsConstantBuffers;
			std::map<size_t, directX::Direct3dBuffer> m_psConstantBuffers;
		};
	}
}
