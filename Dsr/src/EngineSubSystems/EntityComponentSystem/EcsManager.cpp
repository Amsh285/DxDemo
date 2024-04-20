#include "dsrpch.h"
#include "EcsManager.h"

namespace dsr
{
	namespace ecs
	{
		Entity EcsManager::CreateNewEntity()
		{
			static unsigned int lastId = 0;
			static std::mutex entityLock;

			std::lock_guard<std::mutex> guard(entityLock);

			return ++lastId;
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

		void EcsManager::OnUpdate(const dsr::events::UpdateFrameEvent& updateFrameEvent)
		{
			for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
			{
				std::shared_ptr<System> system = *it;
				std::set<Entity> entities = m_systemEntities[system->GetType()];

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

				std::set<Entity> entities = m_systemEntities[renderer->GetType()];

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
				if (HasComponentTypeIntersection(system, entity.second))
					m_systemEntities[system->GetType()].insert(entity.first);
			}
		}

		void EcsManager::UpdateSystemEntityAssignment(
			const std::shared_ptr<System>& system,
			const std::unordered_map<std::type_index, std::shared_ptr<Component>>& componentMap,
			const Entity& entity)
		{
			if (HasComponentTypeIntersection(system, componentMap))
			{
				m_systemEntities[system->GetType()].insert(entity);
			}
		}
	}
}
