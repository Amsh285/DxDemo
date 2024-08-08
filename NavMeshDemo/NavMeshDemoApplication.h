#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"

#include "Data/Manipulation/StaticMeshExtensions.h"

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"

#include "Scenes/RampScene.h"
#include "Scenes/BridgeScene.h"
#include "Scenes/LabyrinthScene.h"

#include "Systems/CameraControllerSystem.h"
#include "Systems/EditorUISystem.h"

class NavMeshDemoApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;
	
	NavMeshDemoApplication();
private:
	dsr::ecs::Entity m_lineEntity;
	dsr::ecs::Entity m_editorUIEntity;

	std::shared_ptr<CameraControllerSystem> m_cameraControllerSystem;
	std::shared_ptr<EditorUISystem> m_editorUISystem;

	void RegisterLineEntity();
	void RegisterCameraController();
	void RegisterEditorUI();

	std::shared_ptr<RampScene> m_rampScene;
	std::shared_ptr<BridgeScene> m_bridgeScene;
	std::shared_ptr<LabyrinthScene> m_labyrinthScene;
};
