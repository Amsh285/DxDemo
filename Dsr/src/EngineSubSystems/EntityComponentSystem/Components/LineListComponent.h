#pragma once

#include "Data/Vertex.h"

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class LineListComponent : public Component
		{
		public:
			std::vector<Vertex3FP4FC> LineList;

		private:
		};
	}
}
