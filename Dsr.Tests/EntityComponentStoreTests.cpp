
#include "pch.h"
#include "dsrpch.h"

#include "EngineSubSystems/EntityComponentSystem/EntityComponentStore.h"
#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TagComponent.h"

using namespace dsr::ecs;

namespace EntityComponentStoreTests
{
	TEST(AddComponent, AddsValidComponent)
	{
		const Entity first = 1;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent = store.AddComponent<NameComponent>(first, "123");
		
		ASSERT_NE(nameComponent, nullptr);
		EXPECT_STREQ(nameComponent->GetName().c_str(), "123");
		EXPECT_EQ(entityComponentMap.size(), 1);
		EXPECT_EQ(entityComponentMap.count(first), 1);
		EXPECT_EQ(entityComponentMap[first].size(), 1);
		EXPECT_EQ(entityComponentMap[first].count(std::type_index(typeid(NameComponent))), 1);
		EXPECT_EQ(entityComponentMap[first][std::type_index(typeid(NameComponent))], nameComponent);

		std::shared_ptr<TagComponent> tagComponent = std::make_shared<TagComponent>("mytag");
		store.AddComponent(first, tagComponent);

		EXPECT_EQ(entityComponentMap.size(), 1);
		EXPECT_EQ(entityComponentMap.count(first), 1);
		EXPECT_EQ(entityComponentMap[first].size(), 2);
		EXPECT_EQ(entityComponentMap[first].count(std::type_index(typeid(TagComponent))), 1);
		EXPECT_EQ(entityComponentMap[first][std::type_index(typeid(TagComponent))], tagComponent);

		const Entity second = 2;
		std::shared_ptr<NameComponent> secondNameComponent = store.AddComponent<NameComponent>(second, "456");

		EXPECT_EQ(entityComponentMap.size(), 2);

		//check integrity of first entity again
		//it should be the same as before because only second had changes
		EXPECT_EQ(entityComponentMap.count(first), 1);
		EXPECT_EQ(entityComponentMap[first].size(), 2);
		EXPECT_EQ(entityComponentMap[first].count(std::type_index(typeid(NameComponent))), 1);
		EXPECT_EQ(entityComponentMap[first][std::type_index(typeid(NameComponent))], nameComponent);
		EXPECT_EQ(entityComponentMap[first].count(std::type_index(typeid(TagComponent))), 1);
		EXPECT_EQ(entityComponentMap[first][std::type_index(typeid(TagComponent))], tagComponent);

		// now check the second Entity that received changes
		EXPECT_EQ(entityComponentMap.count(second), 1);
		EXPECT_EQ(entityComponentMap[second].size(), 1);
		EXPECT_EQ(entityComponentMap[second].count(std::type_index(typeid(NameComponent))), 1);
		EXPECT_EQ(entityComponentMap[second][std::type_index(typeid(NameComponent))], secondNameComponent);
	}
}
