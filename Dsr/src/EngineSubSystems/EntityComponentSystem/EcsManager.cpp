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

		void EcsManager::OnUpdate(const dsr::events::UpdateFrameEvent& updateFrameEvent)
		{
			for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
			{
				std::shared_ptr<System> system = *it;
				std::vector<Entity> entities = m_systemEntities[system->GetType()];

				for (Entity& entity : entities)
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

				std::vector<Entity> entities = m_systemEntities[renderer->GetType()];

				for (Entity& entity : entities)
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
					m_systemEntities[system->GetType()].push_back(entity.first);
			}
		}

		void EcsManager::UpdateSystemEntityAssignment(
			const std::shared_ptr<System>& system,
			const std::unordered_map<std::type_index, std::shared_ptr<Component>>& componentMap,
			const Entity& entity)
		{
			std::vector<Entity>& systemEntityVector = m_systemEntities[system->GetType()];

			//check if the entity is already registered for the current system and if the componentmap of the entity has the components needed by the current system.
			if (std::find(systemEntityVector.begin(), systemEntityVector.end(), entity) == systemEntityVector.end() && HasComponentTypeIntersection(system, componentMap))
			{
				systemEntityVector.push_back(entity);
			}
		}
	}
}
