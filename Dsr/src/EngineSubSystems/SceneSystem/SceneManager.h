#pragma once

#include "ErrorHandling/NotFoundError.h"

#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"
#include "EngineSubSystems/SceneSystem/Scene.h"
#include "EngineSubSystems/SceneSystem/SceneProxy.h"

namespace dsr
{
	namespace scenesystem
	{
		class SceneManager
		{
		public:
			template<class TComponent>
			std::shared_ptr<TComponent> GetComponentFrom(const uint32_t& sceneId,  const dsr::ecs::Entity& entity) const
			{
				return m_scenes.at(sceneId)->GetComponentFrom<TComponent>(entity);
			}

			SceneManager(const std::shared_ptr<dsr::ecs::EcsManager>& ecaManager);
			SceneManager(const SceneManager& other) = delete;
			SceneManager& operator=(const SceneManager& other) = delete;

			dsr::ecs::Entity CreateNewEntity() const;
			
			uint32_t CreateNewScene(const std::string& name);
			void AddScene(const std::shared_ptr<Scene>& scene);

			void RemoveScene(const uint32_t& sceneId);
			void RemoveScene(const std::shared_ptr<Scene>& scene);
			
			template<class TComponent, class ...TArgs>
			std::shared_ptr<TComponent> AddComponent(const uint32_t& sceneId, const dsr::ecs::Entity& entity, TArgs... args)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				if (m_activeScene && m_activeScene->Equals(sceneId))
				{
					return m_activeScene->AddComponent<TComponent>(entity, args...);
				}
				else
				{
					auto it = m_scenes.find(sceneId);

					if (it == m_scenes.end())
						throw NotFoundError("SceneId: " + std::to_string(sceneId) +  " not found.");

					return it->second->AddComponent<TComponent>(entity, args...);
				}
			}

			template<class TComponent>
			void AddComponent(const uint32_t& sceneId, const dsr::ecs::Entity& entity, const std::shared_ptr<TComponent>& component)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				AddComponent(sceneId, entity, std::type_index(typeid(TComponent)), component);
			}

			void AddComponent(
				const uint32_t& sceneId,
				const dsr::ecs::Entity& entity,
				const std::type_index& componentType,
				const std::shared_ptr<dsr::ecs::Component>& component
			);

			void SetActiveScene(const uint32_t& sceneId);
		private:
			std::shared_ptr<dsr::ecs::EcsManager> m_ecsManager;

			std::unique_ptr<SceneProxy> m_activeScene;
			ska::flat_hash_map<uint32_t, std::shared_ptr<Scene>> m_scenes;
		};
	}
}
