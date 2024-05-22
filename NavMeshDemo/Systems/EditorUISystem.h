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

struct SceneViewData
{
	std::string SceneName;
	uint32_t SceneId;
	bool IsSelected;

	SceneViewData()
		: SceneName(""), SceneId(std::numeric_limits<uint32_t>::max()), IsSelected(false)
	{
	}
};

class EditorUISystem : public dsr::ecs::System
{
public:
	virtual std::vector<std::type_index> GetRequiredComponents() const override;

	EditorUISystem(
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::input::Input>& input,
		const std::shared_ptr<RampScene>& rampScene
	);
	EditorUISystem(const EditorUISystem& other) = delete;
	EditorUISystem& operator=(const EditorUISystem& other) = delete;

	void Start(const dsr::ecs::EngineStartupContext& context);
	void Update(const dsr::ecs::EngineContext& context);
private:
	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::input::Input> m_input;
	std::shared_ptr<RampScene> m_rampScene;

	std::vector<SceneViewData> m_sceneViewData;
	size_t m_sceneSelectedIdx;
};

