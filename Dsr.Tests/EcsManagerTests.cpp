
#include "pch.h"

#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"

#include "TestModels/EntityComponentSystem/Components/TestNameComponent.h"
#include "TestModels/EntityComponentSystem/Components/TestTagComponent.h"
#include "TestModels/EntityComponentSystem/Components/TestDummyComponent.h"

#include "TestModels/EntityComponentSystem/Systems/TestNameSystem.h"
#include "TestModels/EntityComponentSystem/Systems/TestNameTagSystem.h"
#include "TestModels/EntityComponentSystem/Systems/TestTagSystem.h"

using namespace dsr::ecs;
using namespace dsr::tests;

namespace EcsManagerTests
{
	class EcsManagerSystemEntityTests : public testing::Test
	{
	public:
		std::vector<Entity> FindSystemAssignedEntities(const std::type_index& systemType) const
		{
			using namespace dsr::ecs;

			std::unordered_map<std::type_index, EcsManager::EntityVectorIndexMapPair> systemEntities = m_ecsManager.GetSystemEntityAssignments();

			auto it = systemEntities.find(systemType);

			if (it == systemEntities.end())
				return std::vector<Entity>();

			return it->second.first;
		}
	protected:
		EcsManagerSystemEntityTests() {
			m_nameEntity = m_ecsManager.CreateNewEntity();
			m_ecsManager.RegisterComponent<TestNameComponent>(m_nameEntity, "name entity");

			m_nameTagEntity = m_ecsManager.CreateNewEntity();
			m_ecsManager.RegisterComponent<TestNameComponent>(m_nameTagEntity, "nameTag entity");
			m_ecsManager.RegisterComponent<TestTagComponent>(m_nameTagEntity, "T1");

			m_dummyEntity = m_ecsManager.CreateNewEntity();
			m_ecsManager.RegisterComponent<TestDummyComponent>(m_dummyEntity);

			m_ecsManager.RegisterSystem<TestNameSystem>();
			m_ecsManager.RegisterSystem<TestNameTagSystem>();
			m_ecsManager.RegisterSystem<TestTagSystem>();
		}


		Entity m_nameEntity;
		Entity m_nameTagEntity;
		Entity m_dummyEntity;

		EcsManager m_ecsManager;
	private:
	};

	TEST_F(EcsManagerSystemEntityTests, Systems_HaveValidEntityEntries)
	{
		std::vector<Entity> nameSystemEntities = FindSystemAssignedEntities(std::type_index(typeid(TestNameSystem)));

		ASSERT_EQ(nameSystemEntities.size(), 2);
		EXPECT_EQ(std::count(nameSystemEntities.begin(), nameSystemEntities.end(), m_nameEntity), 1);
		EXPECT_EQ(std::count(nameSystemEntities.begin(), nameSystemEntities.end(), m_nameTagEntity), 1);

		std::vector<Entity> nameTagSystemEntities = FindSystemAssignedEntities(std::type_index(typeid(TestNameTagSystem)));
		ASSERT_EQ(nameTagSystemEntities.size(), 1);
		EXPECT_EQ(std::count(nameSystemEntities.begin(), nameSystemEntities.end(), m_nameTagEntity), 1);

		std::vector<Entity> tagSystemEntities = FindSystemAssignedEntities(typeid(TestTagSystem));
		ASSERT_EQ(tagSystemEntities.size(), 1);
		EXPECT_EQ(std::count(tagSystemEntities.begin(), tagSystemEntities.end(), m_nameTagEntity), 1);

		std::unordered_map<std::type_index, EcsManager::EntityVectorIndexMapPair> systemEntities = m_ecsManager.GetSystemEntityAssignments();
		EXPECT_EQ(systemEntities.size(), 3);
	}

	TEST_F(EcsManagerSystemEntityTests, RemoveComponent_SystemsEntitiesValid)
	{
		std::vector<Entity> nameTagSystemEntitiesBeforeDelete = FindSystemAssignedEntities(std::type_index(typeid(TestNameTagSystem)));
		ASSERT_EQ(nameTagSystemEntitiesBeforeDelete.size(), 1);

		m_ecsManager.RemoveComponent(m_nameTagEntity, typeid(TestNameComponent));

		std::vector<Entity> nameSystemEntities = FindSystemAssignedEntities(std::type_index(typeid(TestNameSystem)));
		ASSERT_EQ(nameSystemEntities.size(), 1);
		EXPECT_EQ(std::count(nameSystemEntities.begin(), nameSystemEntities.end(), m_nameEntity), 1);

		std::vector<Entity> nameTagSystemEntities = FindSystemAssignedEntities(std::type_index(typeid(TestNameTagSystem)));
		ASSERT_EQ(nameTagSystemEntities.size(), 0);

		std::vector<Entity> tagSystemEntities = FindSystemAssignedEntities(typeid(TestTagSystem));
		ASSERT_EQ(tagSystemEntities.size(), 1);
		EXPECT_EQ(std::count(tagSystemEntities.begin(), tagSystemEntities.end(), m_nameTagEntity), 1);

		std::unordered_map<std::type_index, EcsManager::EntityVectorIndexMapPair> systemEntities = m_ecsManager.GetSystemEntityAssignments();
		EXPECT_EQ(systemEntities.size(), 3);
	}

	TEST_F(EcsManagerSystemEntityTests, RemoveSystem_ClearsCorrespondingSystemEntityAssignment)
	{
		std::vector<Entity> testTagSystemEntities = FindSystemAssignedEntities(std::type_index(typeid(TestTagSystem)));
		ASSERT_EQ(testTagSystemEntities.size(), 1);

		m_ecsManager.RemoveSystem<TestTagSystem>();

		std::unordered_map<std::type_index, EcsManager::EntityVectorIndexMapPair> systemEntityAssignments = m_ecsManager.GetSystemEntityAssignments();

		EXPECT_EQ(systemEntityAssignments.count(typeid(TestTagSystem)), 0);
	}

	TEST_F(EcsManagerSystemEntityTests, RemoveSystem_DoesNotAffectOtherSystemEntityAssignments)
	{
		m_ecsManager.RemoveSystem<TestTagSystem>();

		std::vector<Entity> nameSystemEntityAssignments = FindSystemAssignedEntities(typeid(TestNameSystem));
		std::vector<Entity> nameTagSystemEntityAssignments = FindSystemAssignedEntities(typeid(TestNameTagSystem));

		EXPECT_EQ(nameSystemEntityAssignments.size(), 2);
		EXPECT_EQ(nameTagSystemEntityAssignments.size(), 1);
	}
}
