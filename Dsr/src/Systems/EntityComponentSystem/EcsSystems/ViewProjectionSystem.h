#pragma once

#include "Events/Application/WindowEvents.h"

#include "Systems/EntityComponentSystem/System.h"
#include <Systems/EntityComponentSystem/Components/TransformComponent.h>
#include <Systems/EntityComponentSystem/Components/ViewFrustumComponent.h>
#include <Systems/EntityComponentSystem/Components/ViewProjectionComponent.h>

namespace dsr
{
	namespace ecs
	{
		class ViewProjectionSystem : public System
		{
		public:
			std::vector<std::type_index> GetRequiredComponents() const override;

			ViewProjectionSystem();

			void HandleWindowResized(const events::WindowResizedEvent& resizedEvent);
			void Update(const EngineContext& context);
		private:
			DirectX::XMMATRIX GetProjectionMatrix(const std::shared_ptr<ViewFrustumComponent>& viewFrustum);
			DirectX::XMMATRIX GetViewMatrix(const std::shared_ptr<TransformComponent>& transform);

			float m_aspecRatio;
		};
	}
}
