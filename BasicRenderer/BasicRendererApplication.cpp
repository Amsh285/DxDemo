#include "BasicRendererApplication.h"

dsr::DsrResult BasicRendererApplication::Setup()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::windows;

	DsrResult baseResult = DsrApplication::Setup();

	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<Direct3dVertexBufferf, dsr_error> loadContent = LoadContent();

	if (std::holds_alternative<dsr_error>(loadContent))
	{
		dsr_error& error = std::get<dsr_error>(loadContent);
		std::string message = "error loading content: ";
		message += error.what();
		return DsrResult(message, error.GetResult());
	}

	std::variant<Direct3dShaderProgram, dsr_error> loadShaderProgram = LoadShaderProgram();

	if (std::holds_alternative<dsr_error>(loadShaderProgram))
	{
		dsr_error error = std::get<dsr_error>(loadShaderProgram);
		std::string message = "error loading shader program: ";
		message += error.what();
		return DsrResult(message, error.GetResult());
	}

	Direct3dVertexBufferf vertexBuffer = std::get<Direct3dVertexBufferf>(loadContent);
	Direct3dShaderProgram shaderProgram = std::get<Direct3dShaderProgram>(loadShaderProgram);

	rendering::Direct3dRenderUoW uow(shaderProgram);
	rendering::RenderData uowData(vertexBuffer);
	uow.RenderData.push_back(uowData);
	m_renderer->AddUnitOfWork(uow);

	m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 0.0f, -30.0f, 0.0f);
	//m_mainCamera->Transform.Rotation = DirectX::XMVectorSet(15.0f, 0.0f, 0.0f, 0.0f);
	//m_mainCamera->Transform.Rotation = DirectX::XMVectorSet(0.0f, -15.0f, 0.0f, 0.0f);

	return DsrResult::Success("Setup Successful.");
}

BasicRendererApplication::BasicRendererApplication()
	: DsrApplication(L"Basic renderer", 100, 100, 1280, 768)
{
}

std::variant<dsr::directX::Direct3dVertexBufferf, dsr::dsr_error> BasicRendererApplication::LoadContent()
{
	using namespace dsr;
	using namespace dsr::directX;

	const size_t vertexCount = 8;
	const size_t elementCount = vertexCount * 6;

	VertexPosColor g_Vertices[vertexCount] =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) }  // 7
	};

	float* ptr = reinterpret_cast<float*>(g_Vertices);
	std::vector<float> vertexData(ptr, ptr + elementCount);

	std::vector<uint32_t> indexData =
	{
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		4, 5, 1, 4, 1, 0,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7
	};

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector3f("COLOR");

	std::variant<Direct3dVertexBufferf, dsr_error> loadVertexData = SetupVertexBufferf(m_device, vertexData, indexData, inputLayout);

	if (std::holds_alternative<dsr_error>(loadVertexData))
		return dsr_error::Attach("Error Loading Vertexbuffer: ", std::get<dsr_error>(loadVertexData));

	return std::get<Direct3dVertexBufferf>(loadVertexData);
}

std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> BasicRendererApplication::LoadShaderProgram()
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

	// setup projection matrix for vertexshader
	float clientWidth = static_cast<float>(m_window->GetClientWidth());
	float clientHeight = static_cast<float>(m_window->GetClientHeight());

	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),
		clientWidth / clientHeight,
		0.1f, 100.0f);

	DsrResult setProjectionMatrixResult = SetConstantBuffer(m_device, vertexShader, 0, projectionMatrix);
	if (setProjectionMatrixResult.GetResultStatusCode() != RESULT_SUCCESS)
		return dsr_error(setProjectionMatrixResult.GetResultMessage(), setProjectionMatrixResult.GetResultStatusCode());

	auto pixelShader = std::get<std::shared_ptr<Direct3dShader<ID3D11PixelShader>>>(loadPixelShader);

	Direct3dShaderInputLayout vertexShaderInputLayout;
	uint32_t offset = offsetof(VertexPosColor, Position);
	vertexShaderInputLayout.AddVector3f("POSITION", 0, 0, offset);
	offset = offsetof(VertexPosColor, Color);
	vertexShaderInputLayout.AddVector3f("COLOR", 0, 0, offset);

	std::variant<Direct3dShaderProgram, dsr_error> loadShaderProgram = CreateShaderProgram(
		m_device, vertexShader, pixelShader, vertexShaderInputLayout);

	if (std::holds_alternative<dsr_error>(loadShaderProgram))
	{
		dsr_error error = std::get<dsr_error>(loadShaderProgram);
		return dsr_error::Attach("Could not load Shaderprogram. ", error);
	}

	return std::get<Direct3dShaderProgram>(loadShaderProgram);
}
