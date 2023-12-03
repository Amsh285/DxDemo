#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"
#include "DirectX/Shader/Data/Material.h"
#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"


class ModelloaderApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;

	ModelloaderApplication();
private:
	std::variant<dsr::directX::rendering::GroupedVertexBuffer, dsr::dsr_error> LoadContent();
	std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> LoadShaderProgram();
};
