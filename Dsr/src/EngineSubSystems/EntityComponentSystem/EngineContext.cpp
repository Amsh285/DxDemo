#include "dsrpch.h"
#include "EngineContext.h"

namespace dsr
{
	namespace ecs
	{
		bool EngineContext::Exists(const Entity& entity) const
		{
			return m_entities.find(entity) != m_entities.end();
		}

		std::optional<std::unordered_map<std::type_index, std::shared_ptr<Component>>> EngineContext::GetComponents(const Entity& entity) const
		{
			if (m_entities.find(entity) == m_entities.end())
				return std::nullopt;

			return m_entities.at(entity);
		}
	}
}
