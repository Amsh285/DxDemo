#include "ModelloaderApplication.h"
#include <EngineSubSystems/EntityComponentSystem/Components/NameComponent.h>

constexpr auto MODELNAMES_SORC = "sorc";

dsr::DsrResult ModelloaderApplication::Setup()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<std::map<std::string, ModelConfiguration>, dsr::dsr_error> loadContent = LoadContent();
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

	const std::map<std::string, ModelConfiguration>& content = std::get<std::map<std::string, ModelConfiguration>>(loadContent);
	AddContent(std::get<Direct3dShaderProgram>(loadDefaultShaderProgram), content);
	RegisterSorcModel(content);

	//m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 1.0f, -1.0f, 1.0f);
	m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 1.0f, -1.0f, 1.0f);
	//m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 1.0f, -10.0f, 1.0f);

	return dsr::DsrResult::Success("Setup Successful.");
}

ModelloaderApplication::ModelloaderApplication()
	: DsrApplication(L"Model loader", 100, 100, 1280, 768)
{
	m_sorcEntity = m_ecsManager->CreateNewEntity();
}

std::variant<std::map<std::string, dsr::ModelConfiguration>, dsr::dsr_error> ModelloaderApplication::LoadContent()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::map<std::string, ModelConfiguration> models;

	std::variant<ModelConfiguration, dsr::dsr_error> loadSorcModel = LoadSorcModel();
	if (std::holds_alternative<dsr_error>(loadSorcModel))
		return std::get<dsr_error>(loadSorcModel);

	ModelConfiguration sorcModel = std::get<ModelConfiguration>(loadSorcModel);

	/*std::variant<GroupedVertexBuffer, dsr_error> loadModelResult = LoadWavefrontModel(
			m_device,
			m_blenderModelLoader,
			"Assets/CubeTest/",
			"cubeTest.tobj",
			"cubeTest.mtl");*/

	models[MODELNAMES_SORC] = sorcModel;
	return models;
}

void ModelloaderApplication::AddContent(
	const dsr::directX::Direct3dShaderProgram& defaultShader,
	const std::map<std::string, dsr::ModelConfiguration>& content)
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	rendering::Direct3dRenderUoW uow(defaultShader);

	for (const auto& item : content)
	{
		rendering::RenderData uowData(item.second.GetVertexBuffer());
		uowData.Modelname = item.first;
		uowData.VertexGroups = item.second.GetVertexGroups();
		uowData.Transform = item.second.GlobalTransform;

		uow.RenderData.push_back(uowData);
	}

	m_renderer->AddUnitOfWork(uow);
}

void ModelloaderApplication::RegisterSorcModel(const std::map<std::string, dsr::ModelConfiguration>& content)
{
	const dsr::ModelConfiguration& sorcModel = content.at(MODELNAMES_SORC);

	std::shared_ptr<dsr::ecs::NameComponent> sorcNameComponent = m_ecsManager->RegisterComponent<dsr::ecs::NameComponent>(m_sorcEntity);
	sorcNameComponent->SetName("Sorc");

	std::shared_ptr<dsr::ecs::TransformComponent> sorcTransformComponent = m_ecsManager->RegisterComponent<dsr::ecs::TransformComponent>(m_sorcEntity);
	std::shared_ptr<dsr::ecs::StaticMeshComponent> sorcStaticMeshComponent = m_ecsManager->RegisterComponent<dsr::ecs::StaticMeshComponent>(m_sorcEntity);

	sorcStaticMeshComponent->SetVertexBuffer(sorcModel.GetVertexBuffer());
	sorcStaticMeshComponent->SetVertexGroups(sorcModel.GetVertexGroups());
}

std::variant<dsr::ModelConfiguration, dsr::dsr_error> ModelloaderApplication::LoadSorcModel()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<ModelConfiguration, dsr_error> loadModelResult = LoadWavefrontModel(
		m_device,
		m_blenderModelLoader,
		"Assets/sorc/",
		"sorcwithoutStaff.tobj",
		"Sorceress.mtl");
	if (std::holds_alternative<dsr_error>(loadModelResult))
		return std::get<dsr_error>(loadModelResult);

	ModelConfiguration& sorcModel = std::get<ModelConfiguration>(loadModelResult);
	sorcModel.GlobalTransform.Rotation = DirectX::XMVectorSet(0.0f, 90.0f, 0.0f, 0.0f);

	// Ugly but has to be done lashes are probably nearer an will fuck up the eye texture.
	// this is probably due to wrong blending. Must be fixed later.
	sorcModel.RemoveVertexGroup("lashes");
	sorcModel.ConfigureVertexGroup("face", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psFace.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "face: could not load pixelshader: Assets/sorc/shaders/psFace.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_face_01_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_face_01_n.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_face_01_s.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("upper", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psUpper.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psUpper.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_n.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_s.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> emissionMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_e.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> interpolationMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper_cm.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value() && interpolationMap.has_value() && emissionMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PSTextures2D.push_back(emissionMap.value());
				group->PSTextures2D.push_back(interpolationMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("upper1", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psUpper1.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psUpper1.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper1_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper1_n.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper1_s.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> emissionMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper1_e.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> interpolationMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper1_cm.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value() && interpolationMap.has_value() && emissionMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PSTextures2D.push_back(emissionMap.value());
				group->PSTextures2D.push_back(interpolationMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("upper2", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psUpper2.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psUpper2.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper2_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper2_n.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper2_s.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> emissionMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper2_e.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> interpolationMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_upper2_cm.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value() && interpolationMap.has_value() && emissionMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PSTextures2D.push_back(emissionMap.value());
				group->PSTextures2D.push_back(interpolationMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("lower", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psLower.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psLower.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_lower_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_lower_n.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_lower_s.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> emissionMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_lower_e.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> interpolationMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_lower_cm.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value() && interpolationMap.has_value() && emissionMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PSTextures2D.push_back(emissionMap.value());
				group->PSTextures2D.push_back(interpolationMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("skin_lower", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psSkinLower.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psSkinLower.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_base_lower_d_loc_int.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_base_lower_n_loc_int.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_base_lower_s_loc_int.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("skin_upper", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psSkinUpper.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psSkinUpper.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_base_upper_d_loc_int.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_base_upper_n_loc_int.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_base_upper_s_loc_int.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("pupill", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psPupill.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psPupill.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_av_eyeiris_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);

			if (diffuseMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("helmet", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psHelmet.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psHelmet.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_helmet1_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_helmet1_n.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> specularMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_helmet1_s.tga", group->MaterialName);
			std::optional<Direct3dShaderTexture2D> emissionMap = LoadTexture("Assets/sorc/materials/textures/pc_mem_hr_00_helmet1_e.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value() && specularMap.has_value() && emissionMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PSTextures2D.push_back(specularMap.value());
				group->PSTextures2D.push_back(emissionMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});
	sorcModel.ConfigureVertexGroup("hair", [this](std::shared_ptr<VertexGroup> group) {
		std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psHair.hlsl", "ps_5_0");
		if (std::holds_alternative<dsr_error>(loadPixelShader))
		{
			const dsr_error& error = std::get<dsr_error>(loadPixelShader);
			std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psHair.hlsl. Error: " << error.what() << std::endl;
		}
		else
		{
			std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_05_hair_d.tga", group->MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);
			std::optional<Direct3dShaderTexture2D> normalMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_05_hair_n.tga", group->MaterialName);

			if (diffuseMap.has_value() && normalMap.has_value())
			{
				m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
				group->PSTextures2D.push_back(diffuseMap.value());
				group->PSTextures2D.push_back(normalMap.value());
				group->PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
			}
		}
		});

	//have to probably implement blending to properly render this..
	/*std::variant<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>, dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(m_device, L"Assets/sorc/shaders/psLashes.hlsl", "ps_5_0");
	if (std::holds_alternative<dsr_error>(loadPixelShader))
	{
		const dsr_error& error = std::get<dsr_error>(loadPixelShader);
		std::cout << "upper: could not load pixelshader: Assets/sorc/shaders/psLashes.hlsl. Error: " << error.what() << std::endl;
	}
	else
	{
		std::optional<Direct3dShaderTexture2D> diffuseMap = LoadTexture("Assets/sorc/materials/textures/pc_mg_eyelashes_d.tga", group.MaterialName, D3D11_RESOURCE_MISC_GENERATE_MIPS);

		if (diffuseMap.has_value())
		{
			m_device->GenerateMips(diffuseMap.value().GetShaderResourceViewPtr().get());
			group.PSTextures2D.push_back(diffuseMap.value());
			group.PixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);
		}
	}*/


	return sorcModel;
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
