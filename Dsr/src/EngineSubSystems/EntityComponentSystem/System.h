#pragma once

#include "Events/EventListener.h"
#include "EngineSubSystems/EntityComponentSystem/EngineContext.h"

namespace dsr
{
	namespace ecs
	{
		class System : public dsr::events::EventListener
		{
		public:
			std::function<void(const EngineContext&)> OnStart;
			std::function<void(const EngineContext&)> OnUpdate = [](const EngineContext&) {};
			virtual std::vector<std::type_index> GetRequiredComponents() const = 0;
			const std::type_index& GetType() const { return m_type; }

			int32_t GetSortOrder() const { return m_sortOrder; }
		protected:
			System(const std::type_index& type);
			System(const std::type_index& type, const int32_t& sortOrder);

			int32_t m_sortOrder;
		private:
			std::type_index m_type;
		};
	}
}
