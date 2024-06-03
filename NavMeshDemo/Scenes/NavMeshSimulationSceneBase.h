#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Data/ModelConfiguration.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

constexpr auto ERROR_LOAD_BASEMESH = 100;

constexpr auto ERROR_REGISTER_BASEMESH_SETUPMODEL = 200;

struct NavMeshSimulationSceneSettings
{
	std::filesystem::path AssetBaseDirectory;
	std::filesystem::path BaseMeshFileName;
	std::filesystem::path BaseMeshMaterialFileName;



	NavMeshSimulationSceneSettings()
		: AssetBaseDirectory("Assets/"), BaseMeshFileName(""), BaseMeshMaterialFileName("")
	{
	}
};

class NavMeshSimulationSceneBase
{
public:
	uint32_t GetSceneId() const { return m_sceneId; }
	std::string GetSceneName() const { return m_sceneName; }

	NavMeshSimulationSceneSettings GetSceneSettings() const { return m_sceneSettings; }

	NavMeshSimulationSceneBase(
		const std::string& sceneName,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);

	void SetActive();

	dsr::DsrResult BuildScene();
protected:
	uint32_t m_sceneId;
	std::string m_sceneName;

	NavMeshSimulationSceneSettings m_sceneSettings;

	dsr::ecs::Entity m_baseMeshEntity;
	dsr::ecs::Entity m_upperSurfaceEntity;

	//Todo: rethink if they really should be heap allocated
	std::shared_ptr<dsr::WavefrontModel> m_baseMesh;
	std::shared_ptr<dsr::WavefrontModel> m_upperSurface;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;
	std::shared_ptr<dsr::BlenderModelLoader> m_blenderModelLoader;
private:
	dsr::DsrResult LoadSceneData();
	dsr::DsrResult LoadBaseMesh();
	void LoadUpperSurface();

	dsr::DsrResult RegisterBaseMesh();
};
