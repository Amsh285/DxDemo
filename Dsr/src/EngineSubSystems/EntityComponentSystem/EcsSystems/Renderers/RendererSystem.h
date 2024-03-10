#pragma once

#include "EngineSubSystems/EntityComponentSystem/EnginePrepareRendererContext.h"
#include "EngineSubSystems/EntityComponentSystem/System.h"

namespace dsr
{
	namespace ecs
	{
		class RendererSystem : public System
		{
		public:
			std::function<void(const EnginePrepareRendererContext&)> OnPrepareRendererUpdate = [](const EnginePrepareRendererContext& ctx){};

			RendererSystem(const std::type_index& type);
			RendererSystem(const std::type_index& type, const int32_t& sortOrder);
		private:
		};
	}
}
