#include "dsrpch.h"
#include "SceneProxy.h"

namespace dsr
{
	namespace scenesystem
	{
		SceneProxy::SceneProxy(const std::shared_ptr<Scene>& scene, const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager)
			: m_scene(scene), m_ecsManager(ecsManager)
		{
		}

		void SceneProxy::LoadEntities()
		{
			const Scene::EntityComponentMap& entityMap = m_scene->GetEntityComponents();

			for (auto itEntity = entityMap.begin(); itEntity != entityMap.end(); itEntity++)
			{
				const Scene::ComponentTypeMap& componentMap = itEntity->second;

				for (auto itComponent = componentMap.begin(); itComponent != componentMap.end(); itComponent++)
				{
					
				}
			}
		}

		void SceneProxy::UnloadEntities()
		{
		}
	}
}
