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
		class EngineContext
		{
		public:
			Entity GetCurrentEntity() const { return m_currentEntity; }

			template<class TComponent>
			bool HasComponent() const
			{
				return m_entityComponents.HasComponent<TComponent>(m_currentEntity);
			}

			template<class TComponent>
			bool HasComponent(const Entity& entity) const
			{
				return m_entityComponents.HasComponent<TComponent>(entity);
			}

			bool HasComponent(const Entity& entity, const std::type_index& componentType) const;

			std::vector<Entity> FindEntitiesByTag(const std::string& tag) const;

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponent() const
			{
				return m_entityComponents.GetComponentFrom<TComponent>(m_currentEntity);
			}

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponentFrom(const Entity& entity) const
			{
				return m_entityComponents.GetComponentFrom<TComponent>(entity);
			}

			EntityComponentStore::ComponentTypeMap& GetComponents(const Entity& entity);

			//Todo: have to check if i want to delete copy operations
			~EngineContext() = default;
		protected:
			Entity m_currentEntity = 0;
			EntityComponentStore m_entityComponents;
		private:
		};
	}
}
