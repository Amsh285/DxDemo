#pragma once

#include "Events/Application/WindowEvents.h"

#include "Systems/EntityComponentSystem/EcsEngineContext.h"
#include "Systems/EntityComponentSystem/Entity.h"
#include "Systems/EntityComponentSystem/System.h"

namespace dsr
{
	namespace ecs
	{
		class EcsManager
		{
		public:
			EcsManager();

			template<class TComponent>
			void RegisterComponent(const Entity& entity)
			{
				static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must be derived from Component.");
				RegisterComponent<TComponent>(std::make_shared<TComponent>(), entity);
			}

			template<class TComponent>
			void RegisterComponent(const std::shared_ptr<TComponent>& component, const Entity& entity)
			{
				static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must be derived from Component.");

				//only one of the same component for an entity
				if (m_engineContext->HasComponent<TComponent>(entity))
					return;

				m_engineContext->AddComponent(entity, component);

				std::optional<std::unordered_map<std::type_index, std::shared_ptr<Component>>> getComponentResult = m_engineContext->GetComponents(entity);
				if (!getComponentResult.has_value())
					return;

				const std::unordered_map<std::type_index, std::shared_ptr<Component>> componentMap = getComponentResult.value();

				for (std::shared_ptr<System>& system : m_systems)
				{
					std::vector<Entity>& systemEntityVector = m_systemEntities[system->GetType()];

					//check if the entity is already registered for the current system and if the componentmap of the entity has the components needed by the current system.
					if (std::find(systemEntityVector.begin(), systemEntityVector.end(), entity) == systemEntityVector.end() && HasComponentTypeIntersection(system, componentMap))
					{
						systemEntityVector.push_back(entity);

						if (system->OnComponentRegistered)
						{
							m_engineContext->SetCurrentEntity(entity);
							system->OnComponentRegistered(*m_engineContext);
						}
					}
				}
			}

			template<class TComponent>
			void RemoveComponent(const Entity& entity)
			{
				static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must be derived from Component.");

				if (!m_engineContext->Exists(entity))
					return;

				m_engineContext->RemoveComponent<TComponent>(entity);

				for (const std::shared_ptr<System>& system : m_systems)
				{
					std::vector<Entity>& assignedEntities = m_systemEntities[system->GetType()];
					std::vector<std::type_index> requiredComponentTypes = system->GetRequiredComponents();

					auto matchingEntityIterator = std::find(assignedEntities.begin(), assignedEntities.end(), entity);
					bool hasMatchingEntityId = matchingEntityIterator != assignedEntities.end();
					bool hasMatchingTypeIndex = std::find(requiredComponentTypes.begin(), requiredComponentTypes.end(), std::type_index(typeid(TComponent))) != requiredComponentTypes.end();

					if (hasMatchingEntityId && hasMatchingTypeIndex)
					{
						assignedEntities.erase(matchingEntityIterator);

						if (system->OnComponentRemoved)
						{
							m_engineContext->SetCurrentEntity(entity);
							system->OnComponentRemoved(*m_engineContext);
						}
					}
				}
			}

			template<class TSystem>
			void RegisterSystem()
			{
				static_assert(std::is_base_of<System, TSystem>::value, "TSystem must be derived from System");

				const std::type_index& staticType = std::type_index(typeid(TSystem));

				if (std::find_if(m_systems.begin(), m_systems.end(), [&staticType](const std::shared_ptr<System>& sysPtr) { return sysPtr->GetType() == staticType; }) != m_systems.end())
					return;

				std::shared_ptr<TSystem> sys = std::make_shared<TSystem>();
				m_systems.push_back(sys);
				UpdateSystemEntityAssignment(sys);
				OrderSystems();
			}

			template<class TSystem>
			void RegisterSystem(const std::shared_ptr<TSystem>& system)
			{
				static_assert(std::is_base_of<System, TSystem>::value, "TSystem must be derived from System");

				if (std::find_if(m_systems.begin(), m_systems.end(), [&system](const std::shared_ptr<System>& sysPtr) { return sysPtr->GetType() == system; }) != m_systems.end())
					return;

				m_systems.push_back(system);
				UpdateSystemEntityAssignment(system);
				OrderSystems();
			}

			template<class TSystem>
			void RemoveSystem()
			{
				static_assert(std::is_base_of<System, TSystem>::value, "TSystem must be derived from System");

				const std::type_index& sysType = std::type_index(typeid(TSystem));
				auto sysIterator = std::find_if(m_systems.begin(), m_systems.end(), [&sysType](const std::shared_ptr<System>& sys) {return sys->GetType() == sysType; });

				if (sysIterator != m_systems.end())
					m_systems.erase(sysIterator);

				m_systemEntities.erase(sysType);
			}

			void OnUpdate(const dsr::events::UpdateFrameEvent& updateFrameEvent);

			EcsManager(const EcsManager& other) = delete;
			EcsManager& operator=(const EcsManager& other) = delete;
		private:
			bool HasComponentTypeIntersection(const std::shared_ptr<System>& system, const std::unordered_map<std::type_index, std::shared_ptr<Component>>& componentMap);
			void UpdateSystemEntityAssignment(const std::shared_ptr<System>& system);
			void OrderSystems();

			std::shared_ptr<EcsEngineContext> m_engineContext;

			std::vector<std::shared_ptr<System>> m_systems;
			std::unordered_map<std::type_index, std::vector<Entity>> m_systemEntities;
		};
	}
}
