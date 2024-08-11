#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "NavMeshSimulationSceneBase.h"

class RampScene final : public NavMeshSimulationSceneBase
{
public:
	RampScene(
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);

	void LogTest() const;
private:
	//void RegisterMapFaceNormalsEntity();
};

