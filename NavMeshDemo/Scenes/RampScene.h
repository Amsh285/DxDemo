#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

class RampScene
{
public:
	RampScene(
		const std::shared_ptr<dsr::scenesystem::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);

	void BuildScene();
private:
	uint32_t m_sceneId;
	std::string m_sceneName;

	std::shared_ptr<dsr::scenesystem::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;
	std::shared_ptr<dsr::BlenderModelLoader> m_blenderModelLoader;
};

