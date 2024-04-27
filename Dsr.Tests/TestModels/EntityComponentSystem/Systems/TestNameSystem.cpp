#include "pch.h"
#include "TestNameSystem.h"

namespace dsr
{
	namespace tests
	{
		std::vector<std::type_index> TestNameSystem::GetRequiredComponents() const
		{
			return { std::type_index(typeid(dsr::tests::TestNameComponent)) };
		}

		TestNameSystem::TestNameSystem()
			: dsr::ecs::System(std::type_index(typeid(TestNameSystem)))
		{
		}
	}
}
