#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Data/ModelConfiguration.h"

#include "EngineSubSystems/SceneSystem/Camera.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ErrorHandling/DsrResult.h"

#include "InputDevices/Screen.h"

#include "ModelLoaders/BlenderModelLoader.h"

#include "NavMeshSimulationSceneBase.h"

constexpr auto ERROR_LOADRAMPSCENE_MODELS = 100;

class RampScene final : public NavMeshSimulationSceneBase
{
public:
	RampScene(
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);

private:
	//void RegisterMapFaceNormalsEntity();
};

