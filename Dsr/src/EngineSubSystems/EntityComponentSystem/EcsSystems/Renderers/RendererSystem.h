#pragma once

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/EnginePrepareRendererContext.h"
#include "EngineSubSystems/EntityComponentSystem/System.h"

#include "EngineSubSystems/SceneSystem/Camera.h"

namespace dsr
{
	namespace ecs
	{
		class RendererSystem : public System
		{
		public:
			std::function<void(const EnginePrepareRendererContext&)> OnPrepareRendererUpdate = [](const EnginePrepareRendererContext& ctx){};

			RendererSystem(const std::type_index& type, const std::shared_ptr<directX::Direct3dDevice>& device);
			RendererSystem(const std::type_index& type, const std::shared_ptr<directX::Direct3dDevice>& device, const int32_t& sortOrder);
		protected:
			void SetupCamera();

			std::shared_ptr<directX::Direct3dDevice> m_device;

			std::map<size_t, directX::Direct3dBuffer> m_vsConstantBuffers;
			std::map<size_t, directX::Direct3dBuffer> m_psConstantBuffers;
		private:
		};
	}
}
