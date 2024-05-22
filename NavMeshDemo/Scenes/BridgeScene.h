#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Data/ModelConfiguration.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ErrorHandling/DsrResult.h"

#include "ModelLoaders/BlenderModelLoader.h"

constexpr auto ERROR_LOADBRIDGESCENE_MODELS = 101;

class BridgeScene
{
public:
	BridgeScene(
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);

	dsr::DsrResult BuildScene();
private:
	std::string m_sceneName;
	uint32_t m_sceneId;

	dsr::ecs::Entity m_mapEntity;

	std::shared_ptr<dsr::WavefrontModel> m_mapModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceSubDividedModel;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;
	std::shared_ptr<dsr::BlenderModelLoader> m_blenderModelLoader;

	std::variant<std::vector<dsr::ModelConfiguration>, dsr::dsr_error> LoadSceneData();
	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapModel();
	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapUpperSurfaceModel();

	void RegisterMapModel(const dsr::ModelConfiguration& map);
};

