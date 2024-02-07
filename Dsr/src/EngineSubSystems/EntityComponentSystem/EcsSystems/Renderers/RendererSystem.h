#pragma once

#include "EngineSubSystems/EntityComponentSystem/System.h"

namespace dsr
{
	namespace ecs
	{
		class RendererSystem : public System
		{
		public:
			std::function<void()> OnPrepareRendererUpdate = [](){};

			RendererSystem(const std::type_index& type);
			RendererSystem(const std::type_index& type, const int32_t& sortOrder);
		private:
		};
	}
}
