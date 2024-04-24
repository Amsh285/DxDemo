
#include "pch.h"
#include "dsrpch.h"

#include "EngineSubSystems/EntityComponentSystem/EntityComponentStore.h"
#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"

using namespace dsr::ecs;

namespace EntityComponentStoreTests
{
	TEST(AddComponent, AddsValidComponent)
	{
		const Entity first = 1;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent = store.AddComponent<NameComponent>(first, "123");
		
		EXPECT_NE(nameComponent, nullptr);
		EXPECT_STREQ(nameComponent->GetName().c_str(), "123");
		EXPECT_EQ(entityComponentMap.size(), 1);
		EXPECT_EQ(entityComponentMap.count(first), 1);
	}
}
