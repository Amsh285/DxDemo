#include "ModelloaderApplication.h"

dsr::DsrResult ModelloaderApplication::Setup()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<GroupedVertexBuffer, dsr::dsr_error> loadContent = LoadContent();
	if (std::holds_alternative<dsr_error>(loadContent))
	{
		const dsr_error& error = std::get<dsr_error>(loadContent);
		return DsrResult(error.what(), error.GetResult());
	}

	std::variant<Direct3dShaderProgram, dsr_error> loadShader = LoadShaderProgram();
	if (std::holds_alternative<dsr_error>(loadShader))
	{
		const dsr_error& error = std::get<dsr_error>(loadShader);
		return DsrResult(error.what(), error.GetResult());
	}

	const GroupedVertexBuffer& groupedBuffer = std::get<GroupedVertexBuffer>(loadContent);

	rendering::RenderData uowData(groupedBuffer.Vertexbuffer);
	uowData.VertexGroups = groupedBuffer.VertexGroups;
	uowData.Transform.Rotation = DirectX::XMVectorSet(0.0f, 90.0f, 0.0f, 0.0f);

	rendering::Direct3dRenderUoW uow(std::get<Direct3dShaderProgram>(loadShader));
	uow.RenderData.push_back(uowData);
	m_renderer->AddUnitOfWork(uow);

	m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 1.0f, -3.0f, 0.0f);

	return dsr::DsrResult::Success("Setup Successful.");
}

ModelloaderApplication::ModelloaderApplication()
	: DsrApplication(L"Model loader", 100, 100, 1280, 768)
{
}

std::variant<dsr::directX::rendering::GroupedVertexBuffer, dsr::dsr_error> ModelloaderApplication::LoadContent()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<GroupedVertexBuffer, dsr_error> loadModelResult = LoadWavefrontModel(
		m_device,
		m_blenderModelLoader,
		"Assets/",
		"sorcwithoutStaff.tobj",
		"Sorceress.mtl");

	if (std::holds_alternative<dsr_error>(loadModelResult))
		return std::get<dsr_error>(loadModelResult);

	return std::get<GroupedVertexBuffer>(loadModelResult);
}

std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> ModelloaderApplication::LoadShaderProgram()
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
