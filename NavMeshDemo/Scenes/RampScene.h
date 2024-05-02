#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Data/ModelConfiguration.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ErrorHandling/DsrResult.h"

#include "ModelLoaders/BlenderModelLoader.h"

constexpr auto ERROR_LOADRAMPSCENE_MODELS = 100;

class RampScene
{
public:
	RampScene(
		const std::shared_ptr<dsr::scenesystem::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);

	dsr::DsrResult BuildScene();
	void SetActive();
private:
	std::string m_sceneName;
	uint32_t m_sceneId;
	
	dsr::ecs::Entity m_mapEntity;
	dsr::ecs::Entity m_mapFaceNormalsEntity;
	dsr::ecs::Entity m_mapUpperSurfaceEntity;
	dsr::ecs::Entity m_mapUpperSurfaceSubDividedEntity;
	dsr::ecs::Entity m_pathMarkersEntity;
	dsr::ecs::Entity m_pathEntity;
	dsr::ecs::Entity m_pathSubDividedEntity;

	std::shared_ptr<dsr::WavefrontModel> m_mapModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceSubDividedModel;

	std::shared_ptr<dsr::scenesystem::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;
	std::shared_ptr<dsr::BlenderModelLoader> m_blenderModelLoader;

	std::variant<std::vector<dsr::ModelConfiguration>, dsr::dsr_error> LoadSceneData();
	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapModel();
	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapUpperSurfaceModel();

	void RegisterMapModel(const dsr::ModelConfiguration& map);
	void RegisterMapUpperSurfaceModel(const dsr::ModelConfiguration& mapUpperSurface);
	void RegisterMapUpperSurfaceSubDividedModel();
	void RegisterStartEndMarkerEntities();
	void RegisterPathEntity();
	void RegisterSubDividedPathEntity();
	void RegisterMapFaceNormalsEntity();

	void AddMarkerLine(const dsr::data::Vertex3FP2FTx3FN& vertex, const DirectX::XMMATRIX& transform, std::vector<float>& vertexBufferData);
};

