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

	std::shared_ptr<CameraControllerSystem> m_cameraControllerSystem;

	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapModel();
	void RegisterMapModel(const dsr::ModelConfiguration& map);
	void RegisterCameraController();
};
