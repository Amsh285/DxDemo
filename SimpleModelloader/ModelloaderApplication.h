#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"
#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"


#include "Systems/CameraControllerSystem.h"

class ModelloaderApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;

	ModelloaderApplication();
private:
	void SetupSystems();
	void RegisterCameraSytem();
	void RegisterCameraComponent();

	std::shared_ptr<CameraControllerSystem> m_camerController;

	std::variant<std::map<std::string, dsr::ModelConfiguration>, dsr::dsr_error> LoadContent();

	void RegisterSorcModel(const std::map<std::string, dsr::ModelConfiguration>& content);

	std::variant<dsr::ModelConfiguration, dsr::dsr_error> LoadSorcModel();
	std::optional<dsr::directX::Direct3dShaderTexture2D> LoadTexture(const std::filesystem::path& fileName, const std::string& group = "", const uint32_t& miscFlags = 0);

	dsr::ecs::Entity m_sorcEntity;
};
