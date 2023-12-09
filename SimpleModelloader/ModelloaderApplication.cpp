#include "ModelloaderApplication.h"

/*TODO: Reset Toolset to latest back again!*/

constexpr auto MODELNAMES_SORC = "sorc";

dsr::DsrResult ModelloaderApplication::Setup()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<std::map<std::string, GroupedVertexBuffer>, dsr::dsr_error> loadContent = LoadContent();
	if (std::holds_alternative<dsr_error>(loadContent))
	{
		const dsr_error& error = std::get<dsr_error>(loadContent);
		std::string errorMessage = "Error loading Content: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, error.GetResult());
	}

	std::variant<Direct3dShaderProgram, dsr_error> loadDefaultShaderProgram = LoadDefaultShaderProgram();
	if (std::holds_alternative<dsr_error>(loadDefaultShaderProgram))
	{
		const dsr_error& error = std::get<dsr_error>(loadDefaultShaderProgram);
		return DsrResult(error.what(), error.GetResult());
	}

	AddContent(std::get<Direct3dShaderProgram>(loadDefaultShaderProgram), std::get<std::map<std::string, GroupedVertexBuffer>>(loadContent));

	m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 1.0f, -3.0f, 1.0f);
	//m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 1.0f, -10.0f, 1.0f);

	return dsr::DsrResult::Success("Setup Successful.");
}

ModelloaderApplication::ModelloaderApplication()
	: DsrApplication(L"Model loader", 100, 100, 1280, 768)
{
}

std::variant<std::map<std::string, dsr::directX::rendering::GroupedVertexBuffer>, dsr::dsr_error> ModelloaderApplication::LoadContent()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::map<std::string, GroupedVertexBuffer> models;

	std::variant<GroupedVertexBuffer, dsr::dsr_error> loadSorcModel = LoadSorcModel();
	if (std::holds_alternative<dsr_error>(loadSorcModel))
		return std::get<dsr_error>(loadSorcModel);

	GroupedVertexBuffer sorcModel = std::get<GroupedVertexBuffer>(loadSorcModel);
	sorcModel.GlobalTransform.Rotation = DirectX::XMVectorSet(0.0f, 90.0f, 0.0f, 0.0f);

	for (VertexGroup& group : sorcModel.VertexGroups)
	{
		if (group.MaterialName == "face")
		{
			std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/psFace.hlsl", "ps_5_0");
			if (std::holds_alternative<dsr_error>(loadPixelShader))
			{
				const dsr_error& error = std::get<dsr_error>(loadPixelShader);
				std::cout << "face: could not load pixelshader: Assets/sorc/psFace.hlsl. Error: " << error.what() << std::endl;
			}
			else
			{
				group.PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);

				std::variant<Direct3dShaderTexture2D, dsr_error> loadfaceDiffuseMap = Direct3dShaderTexture2D::LoadSingleRGBA(m_device, "Assets/sorc/materials/textures/pc_mg_av_face_01_d.tga", 1, D3D11_RESOURCE_MISC_GENERATE_MIPS);
				std::variant<Direct3dShaderTexture2D, dsr_error> loadNormalMap = Direct3dShaderTexture2D::LoadSingleRGBA(m_device, "Assets/sorc/materials/textures/pc_mg_av_face_01_n.tga");

				bool hasDiffuseMapError = std::holds_alternative<dsr_error>(loadfaceDiffuseMap);
				bool hasNormalMapError = std::holds_alternative<dsr_error>(loadNormalMap);

				if (hasDiffuseMapError)
				{
					const dsr_error& error = std::get<dsr_error>(loadfaceDiffuseMap);
					std::cout << "face: could not load diffusemap: " << error.what() << std::endl;
				}

				if (hasNormalMapError)
				{
					const dsr_error& error = std::get<dsr_error>(loadNormalMap);
					std::cout << "face: could not load normalmap: " << error.what() << std::endl;
				}

				if (!hasDiffuseMapError && !hasNormalMapError)
				{
					Direct3dShaderTexture2D diffuseMap = std::get<Direct3dShaderTexture2D>(loadfaceDiffuseMap);
					m_device->GenerateMips(diffuseMap.GetShaderResourceViewPtr().get());

					Direct3dShaderTexture2D normalMap = std::get<Direct3dShaderTexture2D>(loadNormalMap);

					group.PSTextures2D.push_back(diffuseMap);
					group.PSTextures2D.push_back(normalMap);
				}
			}
		}
		else if (group.MaterialName == "upper")
		{
			std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/psUpper.hlsl", "ps_5_0");
			if (std::holds_alternative<dsr_error>(loadPixelShader))
			{
				const dsr_error& error = std::get<dsr_error>(loadPixelShader);
				std::cout << "upper: could not load pixelshader: Assets/sorc/psUpper.hlsl. Error: " << error.what() << std::endl;
			}
			else
			{
				std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_d.tga", "upper", D3D11_RESOURCE_MISC_GENERATE_MIPS);
				std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_n.tga", "upper");
				std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_s.tga", "upper");
				std::optional<Direct3dShaderTexture2D> emissionMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_e.tga", "upper");
				std::optional<Direct3dShaderTexture2D> interpolationMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_cm.tga", "upper");

				if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value() && interpolationMap.has_value() && emissionMap.has_value())
				{
					m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
					group.PSTextures2D.push_back(diffuseMap.value());
					group.PSTextures2D.push_back(normalMap.value());
					group.PSTextures2D.push_back(specularMap.value());
					group.PSTextures2D.push_back(emissionMap.value());
					group.PSTextures2D.push_back(interpolationMap.value());
					group.PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
				}
			}
		}
	}

	models[MODELNAMES_SORC] = sorcModel;
	return models;
}

void ModelloaderApplication::AddContent(
	const dsr::directX::Direct3dShaderProgram& defaultShader,
	const std::map<std::string, dsr::directX::rendering::GroupedVertexBuffer>& content)
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	rendering::Direct3dRenderUoW uow(defaultShader);

	for (const auto& item : content)
	{
		rendering::RenderData uowData(item.second.Vertexbuffer);
		uowData.Modelname = item.first;
		uowData.VertexGroups = item.second.VertexGroups;
		uowData.Transform = item.second.GlobalTransform;

		uow.RenderData.push_back(uowData);
	}

	m_renderer->AddUnitOfWork(uow);
}

std::variant<dsr::directX::rendering::GroupedVertexBuffer, dsr::dsr_error> ModelloaderApplication::LoadSorcModel()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<GroupedVertexBuffer, dsr_error> loadModelResult = LoadWavefrontModel(
		m_device,
		m_blenderModelLoader,
		"Assets/sorc/",
		"sorcwithoutStaff.tobj",
		"Sorceress.mtl");

	/*std::variant<GroupedVertexBuffer, dsr_error> loadModelResult = LoadWavefrontModel(
		m_device,
		m_blenderModelLoader,
		"Assets/CubeTest/",
		"cubeTest.tobj",
		"cubeTest.mtl");*/

	if (std::holds_alternative<dsr_error>(loadModelResult))
		return std::get<dsr_error>(loadModelResult);

	return std::get<GroupedVertexBuffer>(loadModelResult);
}

std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> ModelloaderApplication::LoadDefaultShaderProgram()
{
	using namespace dsr;
	using namespace dsr::directX;

	std::variant<std::shared_ptr<Direct3dShader<ID3D11VertexShader>>, dsr_error> loadVertexShader = LoadShaderFromFile<ID3D11VertexShader>(m_device, L"Assets/VertexShader.hlsl", "vs_5_0");
	std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr::dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/PixelShader.hlsl", "ps_5_0");

	if (std::holds_alternative<dsr_error>(loadVertexShader))
	{
		dsr_error error = std::get<dsr_error>(loadVertexShader);
		return dsr_error::Attach("Could not load VertexShader. ", error);
	}

	if (std::holds_alternative<dsr_error>(loadPixelShader))
	{
		dsr_error error = std::get<dsr_error>(loadPixelShader);
		return dsr_error::Attach("Could not load PixelShader. ", error);
	}

	auto vertexShader = std::get<std::shared_ptr<Direct3dShader<ID3D11VertexShader>>>(loadVertexShader);
	auto pixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);

	Direct3dShaderInputLayout vertexShaderInputLayout;
	vertexShaderInputLayout.AddVector3f("POSITION");
	vertexShaderInputLayout.AddVector2f("TXCOORD");
	vertexShaderInputLayout.AddVector3f("NORMAL");

	std::variant<Direct3dShaderProgram, dsr_error> loadShaderProgram = CreateShaderProgram(
		m_device, vertexShader, pixelShader, vertexShaderInputLayout);

	if (std::holds_alternative<dsr_error>(loadShaderProgram))
	{
		dsr_error error = std::get<dsr_error>(loadShaderProgram);
		return dsr_error::Attach("Could not load Shaderprogram. ", error);
	}

	return std::get<Direct3dShaderProgram>(loadShaderProgram);
}

std::optional<dsr::directX::Direct3dShaderTexture2D> ModelloaderApplication::LoadTexture(const std::filesystem::path& fileName, const std::string& group, const uint32_t& miscFlags)
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<Direct3dShaderTexture2D, dsr_error> loadTextureResult = Direct3dShaderTexture2D::LoadSingleRGBA(m_device, fileName, 1, miscFlags);

	if (std::holds_alternative<dsr_error>(loadTextureResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadTextureResult);
		std::cout << group << ": could not load texture: " << fileName << ". Error: " << error.what() << std::endl;
		return std::nullopt;
	}

	return std::get<Direct3dShaderTexture2D>(loadTextureResult);
}
