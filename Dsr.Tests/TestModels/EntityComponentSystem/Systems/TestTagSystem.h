#pragma once

#include "EngineSubSystems/EntityComponentSystem/System.h"

#include "TestModels/EntityComponentSystem/Components/TestTagComponent.h"

namespace dsr
{
	namespace tests
	{
		class TestTagSystem final : public dsr::ecs::System
		{
		public:
			std::vector<std::type_index> GetRequiredComponents() const override;

			TestTagSystem();
		private:
		};
	}
}
