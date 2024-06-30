#pragma once

#include "dsrpch.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/EntityComponentSystem/System.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ViewProjectionComponent.h"

#include "EngineSubSystems/Input/Input.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "Components/EditorUIComponent.h"

#include "Scenes/RampScene.h"
#include "Scenes/Data/Events/EditorScreenClickEvent.h"

class EditorUISystem : public dsr::ecs::System
{
public:
	virtual std::vector<std::type_index> GetRequiredComponents() const override;

	EditorUISystem(
		const std::shared_ptr<dsr::input::Input>& input,
		const std::vector<std::shared_ptr<NavMeshSimulationSceneBase>>& scenes
	);
	EditorUISystem(const EditorUISystem& other) = delete;
	EditorUISystem& operator=(const EditorUISystem& other) = delete;

	//void Start(const dsr::ecs::EngineStartupContext& context);
	void Update(const dsr::ecs::EngineContext& context);
private:
	std::shared_ptr<dsr::input::Input> m_input;
	std::vector<std::shared_ptr<NavMeshSimulationSceneBase>> m_scenes;

	size_t m_sceneSelectedIdx;
	int32_t m_subDivisionCount;
	int32_t m_barycentricSubDivisionCount;
};
