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
