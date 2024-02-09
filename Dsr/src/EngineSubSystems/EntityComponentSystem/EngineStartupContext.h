#pragma once

#include "EngineSubSystems/EntityComponentSystem/EngineContext.h"

namespace dsr
{
	namespace ecs
	{
		class EngineStartupContext final
		{
		public:
			EngineStartupContext(const std::shared_ptr<EngineContext>& context);

			std::vector<Entity> FindEntitiesByTag(const std::string& tag) const;

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponentFrom(const Entity& entity) const
			{
				return m_context->GetComponentFrom<TComponent>(entity);
			}
		private:
			std::shared_ptr<EngineContext> m_context;
		};
	}
}
