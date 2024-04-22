#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"

#include "Data/Manipulation/StaticMeshExtensions.h"
#include "Data/Pathfinding/AStarStaticMeshPathfinder.h"

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"

#include "Systems/CameraControllerSystem.h"
#include "Systems/EditorUISystem.h"

#include "Scenes/RampScene.h"

class NavMeshDemoApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;
	
	NavMeshDemoApplication();
private:
	dsr::ecs::Entity m_mapEntity;
	dsr::ecs::Entity m_mapFaceNormalsEntity;
	dsr::ecs::Entity m_mapUpperSurfaceEntity;
	dsr::ecs::Entity m_mapUpperSurfaceSubDividedEntity;
	dsr::ecs::Entity m_lineEntity;
	dsr::ecs::Entity m_pathMarkersEntity;
	dsr::ecs::Entity m_pathEntity;

	dsr::ecs::Entity m_editorUIEntity;

	std::shared_ptr<CameraControllerSystem> m_cameraControllerSystem;
	std::shared_ptr<EditorUISystem> m_editorUISystem;

	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapModel();
	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapUpperSurfaceModel();

	void RegisterMapModel(const dsr::ModelConfiguration& map);
	void RegisterMapUpperSurfaceModel(const dsr::ModelConfiguration& mapUpperSurface);
	void RegisterMapUpperSurfaceSubDividedModel();
	void RegisterLineEntity();
	void RegisterStartEndMarkerEntities();
	void RegisterPathEntity();
	void RegisterMapFaceNormalsEntity();
	void RegisterCameraController();
	void RegisterEditorUI();

	void AddMarkerLine(const dsr::data::Vertex3FP2FTx3FN& vertex, const DirectX::XMMATRIX& transform, std::vector<float>& vertexBufferData);

	std::shared_ptr<dsr::WavefrontModel> m_mapModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceSubDividedModel;

	std::shared_ptr<RampScene> m_rampScene;
};
