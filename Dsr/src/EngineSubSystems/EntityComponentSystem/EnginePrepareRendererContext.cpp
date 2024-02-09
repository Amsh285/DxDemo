#include "dsrpch.h"
#include "EnginePrepareRendererContext.h"

namespace dsr
{
	namespace ecs
	{
		EnginePrepareRendererContext::EnginePrepareRendererContext(const std::shared_ptr<EngineContext>& context)
			: m_context(context)
		{
		}

		std::vector<Entity> EnginePrepareRendererContext::FindEntitiesByTag(const std::string& tag) const
		{
			return m_context->FindEntitiesByTag(tag);
		}
	}
}
