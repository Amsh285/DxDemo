#pragma once

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/Components/LineListComponent.h"

#include "EngineSubSystems/EntityComponentSystem/EcsSystems/Renderers/RendererSystem.h"

namespace dsr
{
	namespace ecs
	{
		class LineListRendererSystem : public RendererSystem
		{
		public:
			virtual std::vector<std::type_index> GetRequiredComponents() const override;

			LineListRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device);

			void PrepareRendererUpdate();
			void Update(const EngineContext& context);
		private:
			std::shared_ptr<directX::Direct3dDevice> m_device;
		};
	}
}
