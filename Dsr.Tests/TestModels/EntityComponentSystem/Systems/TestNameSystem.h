#pragma once

#include "EngineSubSystems/EntityComponentSystem/System.h"

#include "TestModels/EntityComponentSystem/Components/TestNameComponent.h"

namespace dsr
{
	namespace tests
	{
		class TestNameSystem : public dsr::ecs::System
		{
		public:
			std::vector<std::type_index> GetRequiredComponents() const override;

			TestNameSystem();
		private:
		};
	}
}
