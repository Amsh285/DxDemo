#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "NavMeshSimulationSceneBase.h"

class BridgeScene final : public NavMeshSimulationSceneBase
{
public:
	BridgeScene(
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);
private:
};
