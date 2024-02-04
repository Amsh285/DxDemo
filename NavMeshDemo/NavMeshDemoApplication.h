#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"

class NavMeshDemoApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;
	
	NavMeshDemoApplication();
private:
	dsr::ecs::Entity m_mapEntity;

	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadMapModel();
	void RegisterMapModel(const dsr::ModelConfiguration& map);
};
