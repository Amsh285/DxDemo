#include "dsrpch.h"
#include "EcsManager.h"

namespace dsr
{
	namespace ecs
	{
		EcsManager::EcsManager()
			: m_engineContext(std::make_shared<EcsEngineContext>())
		{
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

		bool EcsManager::HasComponentTypeIntersection(const std::shared_ptr<System>& system, const std::unordered_map<std::type_index, std::shared_ptr<Component>>& componentMap)
		{
			const std::vector<std::type_index>& requiredComponents = system->GetRequiredComponents();

			for (const std::type_index& requiredIndex : requiredComponents)
			{
				if (componentMap.find(requiredIndex) == componentMap.end())
					return false;
			}

			return true;
		}

		void EcsManager::UpdateSystemEntityAssignment(const std::shared_ptr<System>& system)
		{
			for (auto& entity : m_engineContext->GetEntities())
			{
				if (HasComponentTypeIntersection(system, entity.second))
					m_systemEntities[system->GetType()].push_back(entity.first);
			}
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
	}
}
