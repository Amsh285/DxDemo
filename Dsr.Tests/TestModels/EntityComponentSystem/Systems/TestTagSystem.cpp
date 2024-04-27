#include "pch.h"
#include "TestTagSystem.h"

namespace dsr
{
	namespace tests
	{
		std::vector<std::type_index> TestTagSystem::GetRequiredComponents() const
		{
			return { typeid(TestTagComponent) };
		}

		TestTagSystem::TestTagSystem()
			: dsr::ecs::System(typeid(TestTagSystem))
		{
		}
	}
}
