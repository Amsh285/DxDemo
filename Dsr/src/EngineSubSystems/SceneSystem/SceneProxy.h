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
			std::shared_ptr<TComponent> AddComponent(const dsr::ecs::Entity& entity, TArgs... args)
			{
				std::shared_ptr<TComponent> componentPtr = m_scene->AddComponent<TComponent, TArgs...>(entity, args...);
				m_ecsManager->RegisterComponent(componentPtr, entity);
				return componentPtr;
			}

			template<class TComponent>
			void AddComponent(const dsr::ecs::Entity& entity, const std::shared_ptr<TComponent>& component)
			{
				m_scene->AddComponent<TComponent>(component, entity);
				m_ecsManager->RegisterComponent<TComponent>(component, entity);
			}

			void AddComponent(
				const dsr::ecs::Entity& entity,
				const std::type_index& componentType,
				const std::shared_ptr<dsr::ecs::Component>& component
			);

			template<class TComponent>
			void RemoveComponent(const dsr::ecs::Entity& entity)
			{
				m_scene->RemoveComponent<TComponent>(entity);
				m_ecsManager->RemoveComponent<TComponent>(entity);
			}

			void LoadEntities();
			void UnloadEntities();

			bool Equals(const uint32_t& sceneId);
			bool Equals(const std::shared_ptr<Scene>& scene);
		private:
			std::shared_ptr<Scene> m_scene;
			std::shared_ptr<dsr::ecs::EcsManager> m_ecsManager;
		};
	}
}
