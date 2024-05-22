
#include "pch.h"

#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "TestModels/EntityComponentSystem/Components/TestNameComponent.h"
#include "TestModels/EntityComponentSystem/Components/TestTagComponent.h"
#include "TestModels/EntityComponentSystem/Components/TestDummyComponent.h"


using namespace dsr::ecs;
using namespace dsr::scene;
using namespace dsr::tests;

namespace SceneManagerTests
{
	class SceneManagerSynchronizationTests : public testing::Test
	{
	public:
		SceneManagerSynchronizationTests()
		{
			m_ecsManager = std::make_shared<EcsManager>();
			m_sceneManager = std::make_shared<SceneManager>(m_ecsManager);

			m_outsideSceneManager = m_sceneManager->CreateNewEntity();
			m_mainEntity1 = m_sceneManager->CreateNewEntity();
			m_mainEntity2 = m_sceneManager->CreateNewEntity();
			m_mainEntity3 = m_sceneManager->CreateNewEntity();
			m_nextSceneEntity1 = m_sceneManager->CreateNewEntity();
			m_nextSceneEntity2 = m_sceneManager->CreateNewEntity();

			m_emptyScene = m_sceneManager->CreateNewScene("empty");
			m_mainScene = m_sceneManager->CreateNewScene("Main");
			m_nextScene = m_sceneManager->CreateNewScene("next");

			m_sceneManager->AddComponent<TestNameComponent>(m_mainScene, m_mainEntity1, "mainEntity 1");
			m_sceneManager->AddComponent<TestNameComponent>(m_mainScene, m_mainEntity2, "mainEntity 2");
			m_sceneManager->AddComponent<TestTagComponent>(m_mainScene, m_mainEntity2, "m2");
			m_sceneManager->AddComponent<TestTagComponent>(m_mainScene, m_mainEntity3, "m3");

			m_sceneManager->AddComponent<TestNameComponent>(m_nextScene, m_nextSceneEntity1, "nextEntity 1");
			m_sceneManager->AddComponent<TestTagComponent>(m_nextScene, m_nextSceneEntity1, "n1");
			m_sceneManager->AddComponent<TestNameComponent>(m_nextScene, m_nextSceneEntity2, "nextEntity 1");

			m_ecsManager->RegisterComponent<TestDummyComponent>(m_outsideSceneManager);
		}

	protected:
		Entity m_outsideSceneManager;

		Entity m_mainEntity1;
		Entity m_mainEntity2;
		Entity m_mainEntity3;

		Entity m_nextSceneEntity1;
		Entity m_nextSceneEntity2;

		uint32_t m_emptyScene;
		uint32_t m_mainScene;
		uint32_t m_nextScene;

		std::shared_ptr<EcsManager> m_ecsManager;
		std::shared_ptr<SceneManager> m_sceneManager;
	};

	TEST_F(SceneManagerSynchronizationTests, SetActiveScene_SynchronizesWithEcsManager)
	{
		EntityComponentStore::EntityComponentMap& ecsMap = m_ecsManager->GetContext()->GetEntityComponentMap();
		EntityComponentStore::EntityComponentMap& mainSceneMap = m_sceneManager->GetEntityComponentMap(m_mainScene);

		ASSERT_EQ(ecsMap.size(), 1);

		m_sceneManager->SetActiveScene(m_mainScene);

		EXPECT_EQ(ecsMap.size(), 4);
		EXPECT_EQ(ecsMap.at(m_outsideSceneManager).count(typeid(TestDummyComponent)), 1);

		EXPECT_EQ(ecsMap.at(m_mainEntity1).count(typeid(TestNameComponent)), 1);

		EXPECT_EQ(ecsMap.at(m_mainEntity2).count(typeid(TestNameComponent)), 1);
		EXPECT_EQ(ecsMap.at(m_mainEntity2).count(typeid(TestTagComponent)), 1);
		
		EXPECT_EQ(ecsMap.at(m_mainEntity3).count(typeid(TestTagComponent)), 1);
	}

	TEST_F(SceneManagerSynchronizationTests, SetActiveScene_FromMainSceneToEmptyScene_RetainsEntitiesOutside)
	{
		EntityComponentStore::EntityComponentMap& ecsMap = m_ecsManager->GetContext()->GetEntityComponentMap();
		EntityComponentStore::EntityComponentMap& mainSceneMap = m_sceneManager->GetEntityComponentMap(m_mainScene);

		ASSERT_EQ(ecsMap.size(), 1);

		m_sceneManager->SetActiveScene(m_mainScene);
		m_sceneManager->SetActiveScene(m_emptyScene);

		EXPECT_EQ(ecsMap.size(), 1);
		EXPECT_TRUE(ecsMap.contains(m_outsideSceneManager));
		EXPECT_TRUE(ecsMap.at(m_outsideSceneManager).contains(typeid(TestDummyComponent)));
	}

	TEST_F(SceneManagerSynchronizationTests, SetActiveScene_FromMainSceneToNextScene_ContainsNextSceneAndOutside)
	{
		EntityComponentStore::EntityComponentMap& ecsMap = m_ecsManager->GetContext()->GetEntityComponentMap();
		EntityComponentStore::EntityComponentMap& mainSceneMap = m_sceneManager->GetEntityComponentMap(m_mainScene);

		ASSERT_EQ(ecsMap.size(), 1);

		m_sceneManager->SetActiveScene(m_mainScene);
		m_sceneManager->SetActiveScene(m_nextScene);

		EXPECT_EQ(ecsMap.size(), 3);
		EXPECT_TRUE(ecsMap.contains(m_outsideSceneManager));
		EXPECT_TRUE(ecsMap.at(m_outsideSceneManager).contains(typeid(TestDummyComponent)));

		EXPECT_TRUE(ecsMap.contains(m_nextSceneEntity1));
		EXPECT_EQ(ecsMap.at(m_nextSceneEntity1).size(), 2);
		EXPECT_TRUE(ecsMap.at(m_nextSceneEntity1).contains(typeid(TestNameComponent)));
		EXPECT_TRUE(ecsMap.at(m_nextSceneEntity1).contains(typeid(TestTagComponent)));

		EXPECT_TRUE(ecsMap.contains(m_nextSceneEntity2));
		EXPECT_EQ(ecsMap.at(m_nextSceneEntity2).size(), 1);
		EXPECT_TRUE(ecsMap.at(m_nextSceneEntity2).contains(typeid(TestNameComponent)));
	}
}
