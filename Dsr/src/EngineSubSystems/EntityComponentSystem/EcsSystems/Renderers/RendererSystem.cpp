#include "dsrpch.h"
#include "RendererSystem.h"

namespace dsr
{
	namespace ecs
	{
		RendererSystem::RendererSystem(const std::type_index& type)
			: System(type)
		{
		}

		RendererSystem::RendererSystem(const std::type_index& type, const int32_t& sortOrder)
			: System(type, sortOrder)
		{
		}
	}
}
