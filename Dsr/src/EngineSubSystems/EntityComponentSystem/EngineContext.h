#pragma once

#include "Components/TagComponent.h"
#include "Components/ViewProjectionComponent.h"

#include "DirectX/Direct3dDeviceShaderExtensions.h"

#include "EngineSubSystems/EntityComponentSystem/Component.h"
#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/EntityComponentSystem/EntityComponentStore.h"

namespace dsr
{
	namespace ecs
	{
		class EngineContext final : public EntityComponentStore
		{
		public:
			Entity GetCurrentEntity() const { return m_currentEntity; }
			void SetCurrentEntity(const Entity& current) { m_currentEntity = current; }

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponent() const
			{
				return GetComponentFrom<TComponent>(m_currentEntity);
			}
		private:
			Entity m_currentEntity = 0;
		};
	}
}
