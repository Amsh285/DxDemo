#include "NavMeshDemoApplication.h"


NavMeshDemoApplication::NavMeshDemoApplication()
	: DsrApplication(L"Nav Mesh Demo", 100, 100, 1280, 768)
{
	m_mapEntity = dsr::ecs::EcsManager::CreateNewEntity();
	m_lineEntity = dsr::ecs::EcsManager::CreateNewEntity();
}

std::variant<dsr::ModelConfiguration, dsr::dsr_error> NavMeshDemoApplication::LoadMapModel()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<ModelConfiguration, dsr_error> loadMapResult = LoadWavefrontModelConfiguration(
		m_device,
		m_blenderModelLoader,
		"Assets/",
		"map.wf",
		"map.mtl");

	if (std::holds_alternative<dsr_error>(loadMapResult))
		return std::get<dsr_error>(loadMapResult);

	return std::get<ModelConfiguration>(loadMapResult);
}

void NavMeshDemoApplication::RegisterMapModel(const dsr::ModelConfiguration& map)
{
	using namespace dsr::ecs;

	m_ecsManager->RegisterComponent<NameComponent>(m_mapEntity, "map");
	m_ecsManager->RegisterComponent<TagComponent>(m_mapEntity, "map");
	m_ecsManager->RegisterComponent<TransformComponent>(m_mapEntity);

	std::shared_ptr<StaticMeshComponent> mesh = m_ecsManager->RegisterComponent<StaticMeshComponent>(m_mapEntity);
	mesh->SetVertexBuffer(map.GetVertexBuffer());
	mesh->SetVertexGroups(map.GetVertexGroups());
}

void NavMeshDemoApplication::RegisterLineEntity()
{
	using namespace dsr;
	using namespace dsr::directX;

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_ecsManager->RegisterComponent<dsr::ecs::LineListComponent>(m_lineEntity);

	std::vector<Vertex3FP4FC> lineList;
	lineList.push_back(dsr::Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, -100.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	lineList.push_back(dsr::Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, 100.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));

	std::vector<float> vertexData;

	for (const Vertex3FP4FC& vertex : lineList)
	{
		vertexData.push_back(vertex.Position.x);
		vertexData.push_back(vertex.Position.y);
		vertexData.push_back(vertex.Position.z);

		vertexData.push_back(vertex.Color.x);
		vertexData.push_back(vertex.Color.y);
		vertexData.push_back(vertex.Color.z);
		vertexData.push_back(vertex.Color.w);
	}

	std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(m_device, vertexData);
	if (std::holds_alternative<dsr_error>(createVertexBuffer))
	{
		const dsr_error& err = std::get<dsr_error>(createVertexBuffer);
		std::cout << "error creatingVertexBuffer for LineData: " << err.what() << std::endl;
		return;
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	lines->SetVertexCount(lineList.size());
	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
	lines->SetVertexShaderInputLayout(inputLayout);
}

void NavMeshDemoApplication::RegisterCameraController()
{
	m_cameraControllerSystem = std::make_shared<CameraControllerSystem>(GetInput(), m_time);
	m_ecsManager->RegisterSystem(m_cameraControllerSystem);

	m_ecsManager->RegisterComponent<CameraControllerComponent>(m_cameraEntity);
}

dsr::DsrResult NavMeshDemoApplication::Setup()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<ModelConfiguration, dsr_error> loadMapResult = LoadMapModel();
	if (std::holds_alternative<dsr_error>(loadMapResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadMapResult);
		std::string errorMessage = "Error loading Map: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, error.GetResult());
	}

	ModelConfiguration config = std::get<ModelConfiguration>(loadMapResult);
	RegisterMapModel(config);
	RegisterLineEntity();
	RegisterCameraController();

	std::vector<dsr::ecs::Entity> cameraEntities = m_ecsManager->FindEntitiesByTag("Camera");
	std::shared_ptr<dsr::ecs::TransformComponent> cameraTransform = m_ecsManager->GetComponentFrom<dsr::ecs::TransformComponent>(cameraEntities[0]);

	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f);
	cameraTransform->SetPositionVec3(cameraPosition);

	return dsr::DsrResult::Success("Setup Successful.");

	return dsr::DsrResult::Success("");
}
