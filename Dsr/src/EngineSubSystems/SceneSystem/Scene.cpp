#include "dsrpch.h"
#include "Scene.h"

namespace dsr
{
	namespace scenesystem
	{
		std::set<dsr::ecs::Entity> Scene::GetEntities() const
		{
			std::set<dsr::ecs::Entity> result;

			for (auto it = m_entityComponents.begin(); it != m_entityComponents.end(); it++)
			{
				result.insert(it->first);
			}

			return result;
		}

		Scene::Scene()
		{
			static std::atomic<uint32_t> uid = 0;

			m_id = uid.fetch_add(1);
		}
	}
}
