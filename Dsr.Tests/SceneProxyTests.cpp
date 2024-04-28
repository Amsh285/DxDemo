
#include "pch.h"

#include "EngineSubSystems/SceneSystem/SceneProxy.h"


#include "TestModels/EntityComponentSystem/Components/TestNameComponent.h"
#include "TestModels/EntityComponentSystem/Components/TestTagComponent.h"
#include "TestModels/EntityComponentSystem/Components/TestDummyComponent.h"

using namespace dsr::ecs;
using namespace dsr::scenesystem;
using namespace dsr::tests;

namespace SceneProxyTests
{
	class SceneProxySceneSynchronizationTests : public testing::Test
	{
	public:

		template<class TComponent>
		bool IsInSync(const Entity& entity)
		{
			return IsInSync(entity, typeid(TComponent));
		}

		bool IsInSync(const Entity& entity, const std::type_index& componentType)
		{
			return m_scene->HasComponent(entity, componentType) && m_ecsManager->GetContext()->HasComponent(entity, componentType);
		}

		SceneProxySceneSynchronizationTests()
		{
			m_scene = std::make_shared<Scene>("testScene");
			m_ecsManager = std::make_shared<EcsManager>();
			m_sceneProxy = std::make_shared<SceneProxy>(m_scene, m_ecsManager);

			m_first = m_ecsManager->CreateNewEntity();
			m_second = m_ecsManager->CreateNewEntity();

			m_sceneProxy->AddComponent<TestNameComponent>(m_first, "first Entity");
			m_sceneProxy->AddComponent<TestTagComponent>(m_first, "tagged_names");
			m_sceneProxy->AddComponent<TestDummyComponent>(m_first);

			m_sceneProxy->AddComponent<TestNameComponent>(m_second, "second Entity");
		}
	protected:
		Entity m_first, m_second;

		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<EcsManager> m_ecsManager;
		std::shared_ptr<SceneProxy> m_sceneProxy;
	private:
	};

	TEST_F(SceneProxySceneSynchronizationTests, SceneInSyncWithEcsManager)
	{
		EntityComponentStore::EntityComponentMap& ecsMap = m_ecsManager->GetContext()->GetEntityComponentMap();
		EntityComponentStore::EntityComponentMap& sceneMap = m_scene->GetEntityComponentMap();

		ASSERT_EQ(ecsMap.size(), sceneMap.size());
		ASSERT_EQ(ecsMap[m_first].size(), sceneMap[m_first].size());
		ASSERT_EQ(ecsMap[m_second].size(), sceneMap[m_second].size());
		ASSERT_EQ(ecsMap[m_first].size(), 3);
		ASSERT_EQ(ecsMap[m_second].size(), 1);

		EXPECT_TRUE(IsInSync<TestNameComponent>(m_first));
		EXPECT_TRUE(IsInSync<TestTagComponent>(m_first));
		EXPECT_TRUE(IsInSync<TestDummyComponent>(m_first));

		EXPECT_TRUE(IsInSync<TestNameComponent>(m_second));
	}

	TEST_F(SceneProxySceneSynchronizationTests, RemoveComponent_HoldsSynch)
	{
		EntityComponentStore::EntityComponentMap& ecsMap = m_ecsManager->GetContext()->GetEntityComponentMap();
		EntityComponentStore::EntityComponentMap& sceneMap = m_scene->GetEntityComponentMap();

		m_sceneProxy->RemoveComponent<TestDummyComponent>(m_first);

		ASSERT_EQ(ecsMap.size(), sceneMap.size());
		ASSERT_EQ(ecsMap[m_first].size(), sceneMap[m_first].size());
		ASSERT_EQ(ecsMap[m_second].size(), sceneMap[m_second].size());
		ASSERT_EQ(ecsMap[m_first].size(), 2);
		ASSERT_EQ(ecsMap[m_second].size(), 1);

		EXPECT_TRUE(IsInSync<TestNameComponent>(m_first));
		EXPECT_TRUE(IsInSync<TestTagComponent>(m_first));

		EXPECT_TRUE(IsInSync<TestNameComponent>(m_second));
	}

	TEST_F(SceneProxySceneSynchronizationTests, UnloadEntities_RemovesFromEcsManager)
	{
		EntityComponentStore::EntityComponentMap& ecsMap = m_ecsManager->GetContext()->GetEntityComponentMap();
		EntityComponentStore::EntityComponentMap& sceneMap = m_scene->GetEntityComponentMap();

		ASSERT_EQ(ecsMap.size(), 2);
		m_sceneProxy->UnloadEntities();
		EXPECT_EQ(sceneMap.size(), 2);
		EXPECT_EQ(ecsMap.size(), 0);
	}

	TEST_F(SceneProxySceneSynchronizationTests, LoadEntities_AfterUnload_AddsFromSceneToEcsManager)
	{
		EntityComponentStore::EntityComponentMap& ecsMap = m_ecsManager->GetContext()->GetEntityComponentMap();
		EntityComponentStore::EntityComponentMap& sceneMap = m_scene->GetEntityComponentMap();

		ASSERT_EQ(ecsMap.size(), sceneMap.size());
		ASSERT_EQ(ecsMap.at(m_first).size(), sceneMap.at(m_first).size());
		ASSERT_EQ(ecsMap.at(m_second).size(), sceneMap.at(m_second).size());
		ASSERT_TRUE(IsInSync<TestNameComponent>(m_first));
		ASSERT_TRUE(IsInSync<TestTagComponent>(m_first));
		ASSERT_TRUE(IsInSync<TestDummyComponent>(m_first));
		ASSERT_TRUE(IsInSync<TestNameComponent>(m_second));

		m_sceneProxy->UnloadEntities(); 
		m_sceneProxy->LoadEntities();

		EXPECT_EQ(ecsMap.size(), sceneMap.size());
		EXPECT_EQ(ecsMap.at(m_first).size(), sceneMap.at(m_first).size());
		EXPECT_EQ(ecsMap.at(m_second).size(), sceneMap.at(m_second).size());
		EXPECT_TRUE(IsInSync<TestNameComponent>(m_first));
		EXPECT_TRUE(IsInSync<TestTagComponent>(m_first));
		EXPECT_TRUE(IsInSync<TestDummyComponent>(m_first));
		EXPECT_TRUE(IsInSync<TestNameComponent>(m_second));
	}
}
