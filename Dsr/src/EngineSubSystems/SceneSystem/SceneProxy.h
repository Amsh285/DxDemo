#pragma once

#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"
#include "EngineSubSystems/SceneSystem/Scene.h"

namespace dsr
{
	namespace scenesystem
	{
		class SceneProxy
		{
		public:
			SceneProxy(const std::shared_ptr<Scene>& scene, const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager);
			SceneProxy(const SceneProxy& other) = delete;
			SceneProxy& operator=(const SceneProxy& other) = delete;

			template<class TComponent, class ...TArgs>
			void AddComponent(const dsr::ecs::Entity& entity, TArgs... args)
			{
				m_scene->AddComponent<TComponent, TArgs...>(entity, args...);
				m_ecsManager->RegisterComponent<TComponent, TArgs...>(entity, args...);
			}

			template<class TComponent>
			void AddComponent(const std::shared_ptr<TComponent>& component, const dsr::ecs::Entity& entity)
			{
				m_scene->AddComponent<TComponent>(component, entity);
				m_ecsManager->RegisterComponent<TComponent>(component, entity);
			}

			template<class TComponent>
			void RemoveComponent(const dsr::ecs::Entity& entity)
			{
				m_scene->RemoveComponent<TComponent>(entity);
				m_ecsManager->RemoveComponent<TComponent>(entity);
			}

			void LoadEntities();
			void UnloadEntities();
		private:
			std::shared_ptr<Scene> m_scene;
			std::shared_ptr<dsr::ecs::EcsManager> m_ecsManager;
		};
	}
}
