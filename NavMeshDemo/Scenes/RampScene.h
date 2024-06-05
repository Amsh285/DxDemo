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

	void OnScreenClick(const dsr::events::MousePosition& position, const dsr::inputdevices::Screen& screen);

private:
	dsr::ecs::Entity m_mapFaceNormalsEntity;
	dsr::ecs::Entity m_mapBarycentricSubdividedEntity;
	dsr::ecs::Entity m_mapUpperSurfaceSubDividedEntity;
	dsr::ecs::Entity m_pathMarkersEntity;
	dsr::ecs::Entity m_pathEntity;
	dsr::ecs::Entity m_pathSubDividedEntity;
	
	std::shared_ptr<dsr::WavefrontModel> m_barycentricSubdividedModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceSubDividedModel;

	void RegisterMapBarycentricSubDivisionEntity();
	void RegisterMapUpperSurfaceModel(const dsr::ModelConfiguration& mapUpperSurface);
	void RegisterMapUpperSurfaceSubDividedModel();
	void RegisterStartEndMarkerEntities();
	void RegisterPathEntity();
	void RegisterSubDividedPathEntity();

	void RegisterMapFaceNormalsEntity();

	void AddMarkerLine(const dsr::data::Vertex3FP2FTx3FN& vertex, const DirectX::XMMATRIX& transform, std::vector<float>& vertexBufferData);
};

