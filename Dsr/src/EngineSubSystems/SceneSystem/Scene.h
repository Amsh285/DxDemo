#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"
#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/EntityComponentSystem/EntityComponentStore.h"

#include "ErrorHandling/InvalidOperationError.h"

namespace dsr
{
	namespace scenesystem
	{
		class Scene : public dsr::ecs::EntityComponentStore
		{
		public:
			uint32_t GetSceneId() const { return m_id; }

			std::string GetName() const { return m_name; }
			void SetName(const std::string& name) { m_name = name; }

			Scene();
			Scene(const std::string& name);
			Scene(const Scene& other) = delete;
			Scene& operator=(const Scene& other) = delete;
		private:
			uint32_t m_id;
			std::string m_name;
		};
	}
}
