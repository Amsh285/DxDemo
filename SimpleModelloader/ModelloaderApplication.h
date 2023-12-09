#pragma once

#include "dsrpch.h"
#include "DsrApplication.h"
#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"


class ModelloaderApplication : public dsr::DsrApplication
{
public:
	dsr::DsrResult Setup() override;

	ModelloaderApplication();
private:
	std::variant<std::map<std::string, dsr::directX::rendering::GroupedVertexBuffer>, dsr::dsr_error> LoadContent();
	void AddContent(
		const dsr::directX::Direct3dShaderProgram& defaultShader,
		const std::map<std::string, dsr::directX::rendering::GroupedVertexBuffer>& content);

	std::variant<dsr::directX::rendering::GroupedVertexBuffer, dsr::dsr_error> LoadSorcModel();
	std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> LoadDefaultShaderProgram();

	std::optional<dsr::directX::Direct3dShaderTexture2D> LoadTexture(const std::filesystem::path& fileName, const std::string& group = "", const uint32_t& miscFlags = 0);
};
