
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

	TEST(AddComponent, RejectsDuplicateComponents)
	{
		const Entity first = 1;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent1 = store.AddComponent<NameComponent>(first, "123");
		std::shared_ptr<NameComponent> nameComponent2 = store.AddComponent<NameComponent>(first, "456");

		ASSERT_NE(nameComponent1, nullptr);
		ASSERT_NE(nameComponent2, nullptr);

		ASSERT_EQ(entityComponentMap.size(), 1);
		ASSERT_EQ(entityComponentMap.count(first), 1);
		ASSERT_EQ(entityComponentMap[first].size(), 1);
		ASSERT_EQ(entityComponentMap[first].count(std::type_index(typeid(NameComponent))), 1);
		EXPECT_EQ(entityComponentMap[first][std::type_index(typeid(NameComponent))], nameComponent1);
		EXPECT_NE(entityComponentMap[first][std::type_index(typeid(NameComponent))], nameComponent2);
	}

	TEST(RemoveComponent, RemovesValidComponent)
	{
		const Entity first = 1;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent = store.AddComponent<NameComponent>(first, "123");
		std::shared_ptr<TagComponent> tagComponent = store.AddComponent<TagComponent>(first, "myTag");

		ASSERT_EQ(entityComponentMap[first].size(), 2);
		ASSERT_EQ(entityComponentMap[first].count(std::type_index(typeid(NameComponent))), 1);
		ASSERT_EQ(entityComponentMap[first].count(std::type_index(typeid(TagComponent))), 1);

		store.RemoveComponent<TagComponent>(first);

		ASSERT_EQ(entityComponentMap[first].size(), 1);
		ASSERT_EQ(entityComponentMap[first].count(std::type_index(typeid(NameComponent))), 1);
		ASSERT_EQ(entityComponentMap[first].count(std::type_index(typeid(TagComponent))), 0);
	}

	TEST(ClearEntity, RemovesValidEntity)
	{
		const Entity first = 1;
		const Entity second = 2;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent1 = store.AddComponent<NameComponent>(first, "123");
		std::shared_ptr<TagComponent> tagComponent = store.AddComponent<TagComponent>(first, "myTag");

		std::shared_ptr<NameComponent> nameComponent2 = store.AddComponent<NameComponent>(second, "456");

		ASSERT_EQ(entityComponentMap.size(), 2);
		ASSERT_EQ(entityComponentMap.count(first), 1);
		ASSERT_EQ(entityComponentMap.count(second), 1);

		store.Clear(second);

		ASSERT_EQ(entityComponentMap.size(), 1);
		ASSERT_EQ(entityComponentMap.count(first), 1);
		ASSERT_EQ(entityComponentMap.count(second), 0);

		EXPECT_EQ(entityComponentMap[first].count(std::type_index(typeid(NameComponent))), 1);
		EXPECT_EQ(entityComponentMap[first].count(std::type_index(typeid(TagComponent))), 1);
	}

	TEST(Clear, RemovesAllEntities)
	{
		const Entity first = 1;
		const Entity second = 2;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent1 = store.AddComponent<NameComponent>(first, "123");
		std::shared_ptr<TagComponent> tagComponent = store.AddComponent<TagComponent>(first, "myTag");

		std::shared_ptr<NameComponent> nameComponent2 = store.AddComponent<NameComponent>(second, "456");

		ASSERT_EQ(entityComponentMap.size(), 2);
		ASSERT_EQ(entityComponentMap.count(first), 1);
		ASSERT_EQ(entityComponentMap.count(second), 1);

		store.Clear();

		ASSERT_EQ(entityComponentMap.size(), 0);
		ASSERT_EQ(entityComponentMap.count(first), 0);
		ASSERT_EQ(entityComponentMap.count(second), 0);
	}

	TEST(GetComponentFrom, ReturnsValidComponent)
	{
		const Entity first = 1;
		const Entity second = 2;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent1 = store.AddComponent<NameComponent>(first, "123");
		std::shared_ptr<TagComponent> tagComponent = store.AddComponent<TagComponent>(first, "myTag");

		std::shared_ptr<NameComponent> nameComponent2 = store.AddComponent<NameComponent>(second, "456");

		std::optional<std::shared_ptr<NameComponent>> actual1 = store.GetComponentFrom<NameComponent>(first);
		std::optional<std::shared_ptr<NameComponent>> actual2 = store.GetComponentFrom<NameComponent>(second);

		ASSERT_TRUE(actual1.has_value());
		ASSERT_TRUE(actual2.has_value());
		EXPECT_EQ(actual1.value(), nameComponent1);
		EXPECT_EQ(actual2.value(), nameComponent2);
	}

	TEST(GetComponentFrom, EntityNotFoundReturnsNullopt)
	{
		const Entity first = 1;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent = store.AddComponent<NameComponent>(first, "123");
		std::shared_ptr<TagComponent> tagComponent = store.AddComponent<TagComponent>(first, "myTag");

		std::optional<std::shared_ptr<NameComponent>> actual = store.GetComponentFrom<NameComponent>(123);

		EXPECT_FALSE(actual.has_value());
	}

	TEST(GetComponentFrom, ComponentNotFoundReturnsNullopt)
	{
		const Entity first = 1;

		EntityComponentStore store;
		EntityComponentStore::EntityComponentMap& entityComponentMap = store.GetEntityComponentMap();

		std::shared_ptr<NameComponent> nameComponent = store.AddComponent<NameComponent>(first, "123");

		std::optional<std::shared_ptr<TagComponent>> actual = store.GetComponentFrom<TagComponent>(123);

		EXPECT_FALSE(actual.has_value());
	}
}
