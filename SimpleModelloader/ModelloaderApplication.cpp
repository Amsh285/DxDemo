#include "ModelloaderApplication.h"

dsr::DsrResult ModelloaderApplication::Setup()
{
	using namespace dsr;
	using namespace dsr::directX;

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<Direct3dVertexBufferf, dsr_error> loadContent = LoadContent();
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

	rendering::Direct3dRenderUoW uow(std::get<Direct3dShaderProgram>(loadShader));
	rendering::RenderData uowData(std::get<Direct3dVertexBufferf>(loadContent));
	uowData.Transform.Rotation = DirectX::XMVectorSet(0.0f, 90.0f, 0.0f, 0.0f);
	uow.RenderData.push_back(uowData);
	m_renderer->AddUnitOfWork(uow);

	m_mainCamera->Transform.Position = DirectX::XMVectorSet(0.0f, 1.0f, -3.0f, 0.0f);

	return dsr::DsrResult::Success("Setup Successful.");
}

ModelloaderApplication::ModelloaderApplication()
	: DsrApplication(L"Model loader", 100, 100, 1280, 768)
{
}

std::variant<dsr::directX::Direct3dVertexBufferf, dsr::dsr_error> ModelloaderApplication::LoadContent()
{
	using namespace dsr;
	using namespace dsr::directX;

	//std::variant<BlenderModel, dsr_error> loadModel = m_blenderModelLoader->Load(L"Assets/Map.obj");
	std::variant<BlenderModel, dsr_error> loadModel = m_blenderModelLoader->Load(
		L"Assets/sorcwithoutStaff.tobj",
		L"Assets/Sorceress.mtl");

	if (std::holds_alternative<dsr_error>(loadModel))
		return dsr::dsr_error::Attach("Error loading blendermodel: ", std::get<dsr_error>(loadModel));

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector2f("TXCOORD");
	inputLayout.AddVector3f("NORMAL");

	const BlenderModel& model = std::get<BlenderModel>(loadModel);
	std::vector<float> vertexBuffer;

	for (const Vertex3FP2FTx3FN& vertex : model.VertexBuffer)
	{
		vertexBuffer.push_back(vertex.Position.x);
		vertexBuffer.push_back(vertex.Position.y);
		vertexBuffer.push_back(vertex.Position.z);

		vertexBuffer.push_back(vertex.texCoords.x);
		vertexBuffer.push_back(vertex.texCoords.y);

		vertexBuffer.push_back(vertex.Normal.x);
		vertexBuffer.push_back(vertex.Normal.y);
		vertexBuffer.push_back(vertex.Normal.z);
	}

	std::vector<rendering::VertexGroup> vertexGroups;

	for (const auto& item : model.MaterialGroups)
	{
		Material data;
		data.Name = item.MaterialName;
		data.SpecularExponent = item.MaterialData.SpecularExponent;
		data.AmbientColor = item.MaterialData.AmbientColor;
		data.DiffuseColor = item.MaterialData.DiffuseColor;
		data.EmissiveColor = item.MaterialData.EmissiveColor;
		data.OpticalDensity = item.MaterialData.OpticalDensity;
		data.IlluminationModel = item.MaterialData.IlluminationModel;

		if (item.MaterialData.MapDiffuse.empty())
			data.DiffuseMap = std::nullopt;
		else
		{
			std::variant<Direct3dShaderTexture2D, dsr_error> loadTextureResult = Direct3dShaderTexture2D::LoadSingleRGBA(m_device, "Assets/" + item.MaterialData.MapDiffuse);

			if (std::holds_alternative<dsr_error>(loadTextureResult))
			{
				dsr_error& error = std::get<dsr_error>(loadTextureResult);
				std::cout << "error loading Texture: " << item.MaterialData.MapDiffuse << ". " << error.what() << std::endl;
				data.DiffuseMap = std::nullopt;
			}
			else
				data.DiffuseMap = std::get<Direct3dShaderTexture2D>(loadTextureResult);
		}

		rendering::VertexGroup group;
		group.StartIndexLocation = item.StartIndexLocation;
		group.IndexCount = item.IndexCount;
		group.Material = data;

		vertexGroups.push_back(group);
	}

	std::variant<Direct3dVertexBufferf, dsr_error> loadVertexData = SetupVertexBufferf(m_device, vertexBuffer, model.IndexBuffer, inputLayout);
	if (std::holds_alternative<dsr_error>(loadVertexData))
		return dsr_error::Attach("Error setup vertexbuffer: ", std::get<dsr_error>(loadVertexData));

	return std::get<Direct3dVertexBufferf>(loadVertexData);
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
