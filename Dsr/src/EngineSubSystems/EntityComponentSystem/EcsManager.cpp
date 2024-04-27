#include "dsrpch.h"
#include "EcsManager.h"

namespace dsr
{
	namespace ecs
	{
		Entity EcsManager::CreateNewEntity()
		{
			static std::atomic<Entity> lastId = 1;

			return lastId.fetch_add(1);
		}

		EcsManager::EcsManager()
			: m_engineContext(std::make_shared<EcsEngineContext>())
		{
		}

		void EcsManager::OrderSystems()
		{
			std::sort(
				m_systems.begin(),
				m_systems.end(),
				[](const std::shared_ptr<System>& left, const std::shared_ptr<System>& right) {
					return left->GetSortOrder() < right->GetSortOrder();
				});
		}

		void EcsManager::RaiseSystemStartEvents()
		{
			m_engineContext->SetCurrentEntity(0);
			EngineStartupContext startupContext(m_engineContext);

			for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
			{
				if ((*it)->OnStart)
				{
					(*it)->OnStart(startupContext);
				}
			}

			for (auto it = m_renderers.begin(); it != m_renderers.end(); ++it)
			{
				if ((*it)->OnStart)
				{
					(*it)->OnStart(startupContext);
				}
			}
		}

		void EcsManager::RegisterComponent(const Entity& entity, const std::type_index& componentType, const std::shared_ptr<Component>& component)
		{
			m_engineContext->AddComponent(entity, componentType, component);

			std::unordered_map<std::type_index, std::shared_ptr<Component>>& componentMap = m_engineContext->GetComponents(entity);

			for (std::shared_ptr<System>& system : m_systems)
			{
				UpdateSystemEntityAssignment(system, componentMap, entity);
			}

			for (std::shared_ptr<RendererSystem>& renderer : m_renderers)
			{
				UpdateSystemEntityAssignment(renderer, componentMap, entity);
			}
		}

		void EcsManager::RemoveComponent(const Entity& entity, const std::type_index& componentType)
		{
			m_engineContext->RemoveComponent(entity, componentType);

			for (const std::shared_ptr<System>& system : m_systems)
			{
				std::pair<std::vector<Entity>, ska::flat_hash_map<Entity, size_t>>& systemEntities = m_systemEntities[system->GetType()];

				std::vector<Entity>& entityVec = systemEntities.first;
				ska::flat_hash_map<Entity, size_t>& entityIndexMap = systemEntities.second;

				if (entityIndexMap.count(entity) > 0 && !HasComponentTypeIntersection(system, m_engineContext->GetComponents(entity)))
				{
					entityVec.erase(entityVec.begin() + entityIndexMap[entity]);
					entityIndexMap.erase(entity);
				}
			}
		}

		void EcsManager::RemoveSystem(const std::type_index& sysType)
		{
			auto sysIterator = std::find_if(m_systems.begin(), m_systems.end(), [&sysType](const std::shared_ptr<System>& sys) {return sys->GetType() == sysType; });

			if (sysIterator != m_systems.end())
				m_systems.erase(sysIterator);

			m_systemEntities.erase(sysType);
		}

		void EcsManager::OnUpdate(const dsr::events::UpdateFrameEvent& updateFrameEvent)
		{
			for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
			{
				std::shared_ptr<System> system = *it;
				const std::vector<Entity>& entities = m_systemEntities[system->GetType()].first;

				for (const Entity& entity : entities)
				{
					m_engineContext->SetCurrentEntity(entity);
					system->OnUpdate(*m_engineContext);
				}
			}
		}

		void EcsManager::OnRendererUpdate(const dsr::events::UpdateFrameEvent& updateFrameEvent)
		{
			for (auto it = m_renderers.begin(); it != m_renderers.end(); ++it)
			{
				std::shared_ptr<RendererSystem> renderer = *it;

				m_engineContext->SetCurrentEntity(0);
				renderer->OnPrepareRendererUpdate(EnginePrepareRendererContext(m_engineContext));

				const std::vector<Entity>& entities = m_systemEntities[renderer->GetType()].first;

				for (const Entity& entity : entities)
				{
					m_engineContext->SetCurrentEntity(entity);
					renderer->OnUpdate(*m_engineContext);
				}
			}
		}

		bool EcsManager::HasComponentTypeIntersection(const std::shared_ptr<System>& system, const std::unordered_map<std::type_index, std::shared_ptr<Component>>& componentMap)
		{
			const std::vector<std::type_index>& requiredComponents = system->GetRequiredComponents();

			for (const std::type_index& requiredIndex : requiredComponents)
			{
				if (componentMap.find(requiredIndex) == componentMap.end())
					return false;
			}

			return !requiredComponents.empty();
		}

		void EcsManager::UpdateSystemEntityAssignment(const std::shared_ptr<System>& system)
		{
			for (auto& entity : m_engineContext->GetEntityComponents())
			{
				UpdateSystemEntityAssignment(system, entity.second, entity.first);
			}
		}

		void EcsManager::UpdateSystemEntityAssignment(
			const std::shared_ptr<System>& system,
			const std::unordered_map<std::type_index, std::shared_ptr<Component>>& componentMap,
			const Entity& entity)
		{
			std::pair<std::vector<Entity>, ska::flat_hash_map<Entity, size_t>>& systemEntities = m_systemEntities[system->GetType()];

			std::vector<Entity>& entityVec = systemEntities.first;
			ska::flat_hash_map<Entity, size_t>& entityIndexMap = systemEntities.second;

			if (entityIndexMap.count(entity) == 0 && HasComponentTypeIntersection(system, componentMap))
			{
				entityVec.push_back(entity);
				entityIndexMap[entity] = entityVec.size() - 1;
			}
		}
	}
}
