#include "dsrpch.h"
#include "EngineStartupContext.h"

namespace dsr
{
	namespace ecs
	{
		EngineStartupContext::EngineStartupContext(const std::shared_ptr<EngineContext>& context)
			: m_context(context)
		{
		}

		std::vector<Entity> EngineStartupContext::FindEntitiesByTag(const std::string& tag) const
		{
			return m_context->FindEntitiesByTag(tag);
		}
	}
}
