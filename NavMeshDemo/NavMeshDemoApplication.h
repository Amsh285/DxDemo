#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"
#include "Systems/CameraControllerSystem.h"

class NavMeshDemoApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;
	
	NavMeshDemoApplication();
private:
	dsr::ecs::Entity m_mapEntity;
	dsr::ecs::Entity m_mapFaceNormalsEntity;
	dsr::ecs::Entity m_mapUpperSurfaceEntity;
	dsr::ecs::Entity m_lineEntity;

	std::shared_ptr<CameraControllerSystem> m_cameraControllerSystem;
	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapModel();
	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapUpperSurfaceModel();

	void RegisterMapModel(const dsr::ModelConfiguration& map);
	void RegisterMapUpperSurfaceModel(const dsr::ModelConfiguration& mapUpperSurface);
	void RegisterLineEntity();
	void RegisterMapFaceNormalsEntity();
	void RegisterCameraController();

	std::shared_ptr<dsr::WavefrontModel> m_mapModel;
	std::shared_ptr<dsr::WavefrontModel> m_mapUpperSurfaceModel;
};
