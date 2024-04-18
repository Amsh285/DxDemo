#pragma once

#include "dsrpch.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/EntityComponentSystem/System.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ViewProjectionComponent.h"

#include "EngineSubSystems/Input/Input.h"

#include "Components/EditorUIComponent.h"

class EditorUISystem : public dsr::ecs::System
{
public:
	virtual std::vector<std::type_index> GetRequiredComponents() const override;

	EditorUISystem(const std::shared_ptr<dsr::input::Input>& input);
	EditorUISystem(const EditorUISystem& other) = delete;
	EditorUISystem& operator=(const EditorUISystem& other) = delete;

	void Update(const dsr::ecs::EngineContext& context);
private:
	std::shared_ptr<dsr::input::Input> m_input;
};

