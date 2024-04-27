#include "pch.h"
#include "TestNameTagSystem.h"

namespace dsr
{
	namespace tests
	{
		std::vector<std::type_index> TestNameTagSystem::GetRequiredComponents() const
		{
			return {
				std::type_index(typeid(TestNameComponent)),
				std::type_index(typeid(TestTagComponent))
			};
		}

		TestNameTagSystem::TestNameTagSystem()
			: dsr::ecs::System(std::type_index(typeid(TestNameTagSystem)))
		{
		}
	}
}
