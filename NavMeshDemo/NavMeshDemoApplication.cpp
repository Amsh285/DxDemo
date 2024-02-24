#include "NavMeshDemoApplication.h"


NavMeshDemoApplication::NavMeshDemoApplication()
	: DsrApplication(L"Nav Mesh Demo", 100, 100, 1280, 768)
{
	m_mapEntity = dsr::ecs::EcsManager::CreateNewEntity();
	m_mapFaceNormalsEntity = dsr::ecs::EcsManager::CreateNewEntity();
	m_mapUpperSurfaceEntity = dsr::ecs::EcsManager::CreateNewEntity();
	m_lineEntity = dsr::ecs::EcsManager::CreateNewEntity();
}

std::variant<dsr::ModelConfiguration, dsr::dsr_error> NavMeshDemoApplication::LoadMapModel()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<std::shared_ptr<WavefrontModel>, dsr_error> loadMapWfResult = LoadWavefrontModel(m_blenderModelLoader,
		"Assets/",
		"map.wf",
		"map.mtl");

	if (std::holds_alternative<dsr_error>(loadMapWfResult))
		return std::get<dsr_error>(loadMapWfResult);

	m_mapModel = std::get<std::shared_ptr<WavefrontModel>>(loadMapWfResult);

	return LoadWavefrontModelConfiguration(
		m_device,
		m_mapModel
	);
}

std::variant<dsr::ModelConfiguration, dsr::dsr_error> NavMeshDemoApplication::LoadMapUpperSurfaceModel()
{
	using namespace dsr;

	using namespace dsr::data;
	using namespace dsr::data::manipulation;

	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	using namespace DirectX;

	std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> filteredMesh = FilterByNormalAngles(
		m_mapModel->Mesh,
		XMConvertToRadians(45.0f)
	);

	std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> subdivided = SubDivide(filteredMesh);

	m_mapUpperSurfaceModel = std::make_shared<WavefrontModel>();
	m_mapUpperSurfaceModel->Mesh = filteredMesh;

	//Todo: Default VertexGroup
	WavefrontModelMaterialGroup group;
	group.IndexCount = m_mapUpperSurfaceModel->Mesh->GetIndexBuffer().size();
	group.StartIndexLocation = 0;
	group.MaterialName = "mat";
	group.MaterialData.SpecularColor = XMFLOAT3(0.8f, 0.8f, 0.8f);
	group.MaterialData.DiffuseColor = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_mapUpperSurfaceModel->MaterialGroups.push_back(group);

	return LoadWavefrontModelConfiguration(
		m_device,
		m_mapUpperSurfaceModel
	);
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

void NavMeshDemoApplication::RegisterMapUpperSurfaceModel(const dsr::ModelConfiguration& mapUpperSurface)
{
	using namespace dsr::ecs;

	// still not completly right stuff gets culled check that later
	// the mesh seems to be looking alright at least

	m_ecsManager->RegisterComponent<NameComponent>(m_mapUpperSurfaceEntity, "map upper surface");
	m_ecsManager->RegisterComponent<TagComponent>(m_mapUpperSurfaceEntity, "map_upper_surface");
	std::shared_ptr<TransformComponent> transform = m_ecsManager->RegisterComponent<TransformComponent>(m_mapUpperSurfaceEntity);
	transform->SetPosition(DirectX::XMVectorSet(0.0f, 40.0f, 0.0f, 1.0f));

	/*std::shared_ptr<StaticMeshComponent> mesh = m_ecsManager->RegisterComponent<StaticMeshComponent>(m_mapUpperSurfaceEntity);
	mesh->SetVertexBuffer(mapUpperSurface.GetVertexBuffer());
	mesh->SetVertexGroups(mapUpperSurface.GetVertexGroups());*/

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(mapUpperSurface.GetVertexBuffer());
	mesh->SetVertexGroups(mapUpperSurface.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_ecsManager->RegisterComponent<WireframeMeshComponent>(m_mapUpperSurfaceEntity);
	wireframe->SetMesh(mesh);
}

void NavMeshDemoApplication::RegisterLineEntity()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::directX;

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_ecsManager->RegisterComponent<dsr::ecs::LineListComponent>(m_lineEntity);

	std::vector<Vertex3FP4FC> lineList;
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, -100.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f)));
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, 100.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f)));

	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, 0.0f, -100.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.8f, 1.0f)));
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.8f, 1.0f)));

	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(-100.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f)));
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(100.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f)));

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

void NavMeshDemoApplication::RegisterMapFaceNormalsEntity()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::vector<Face> mapFaces = GetFaceData(m_mapModel);
	std::vector<float> vertexData;

	for (const Face& face : mapFaces)
	{
		vertexData.push_back(face.Centroid.x);
		vertexData.push_back(face.Centroid.y);
		vertexData.push_back(face.Centroid.z);

		vertexData.push_back(0.0f);
		vertexData.push_back(1.0f);
		vertexData.push_back(0.0f);
		vertexData.push_back(1.0f);

		vertexData.push_back(face.Centroid.x + face.Normal.x);
		vertexData.push_back(face.Centroid.y + face.Normal.y);
		vertexData.push_back(face.Centroid.z + face.Normal.z);

		vertexData.push_back(0.0f);
		vertexData.push_back(1.0f);
		vertexData.push_back(0.0f);
		vertexData.push_back(1.0f);
	}

	std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(m_device, vertexData);
	if (std::holds_alternative<dsr_error>(createVertexBuffer))
	{
		const dsr_error& err = std::get<dsr_error>(createVertexBuffer);
		std::cout << "error creatingVertexBuffer for MapFaceNormals: " << err.what() << std::endl;
		return;
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_ecsManager->RegisterComponent<dsr::ecs::LineListComponent>(m_mapFaceNormalsEntity);
	lines->SetVertexCount(mapFaces.size() * 2);
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

	std::variant<ModelConfiguration, dsr_error> loadMapUpperSurfaceResult = LoadMapUpperSurfaceModel();
	if (std::holds_alternative<dsr_error>(loadMapUpperSurfaceResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadMapUpperSurfaceResult);
		std::string errorMessage = "Error loading Map Upper Surface: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, error.GetResult());
	}

	ModelConfiguration mapUpperSurfaceConfig = std::get<ModelConfiguration>(loadMapUpperSurfaceResult);

	RegisterMapModel(config);
	RegisterMapUpperSurfaceModel(mapUpperSurfaceConfig);
	RegisterLineEntity();
	RegisterMapFaceNormalsEntity();
	RegisterCameraController();

	std::vector<dsr::ecs::Entity> cameraEntities = m_ecsManager->FindEntitiesByTag("Camera");
	std::shared_ptr<dsr::ecs::TransformComponent> cameraTransform = m_ecsManager->GetComponentFrom<dsr::ecs::TransformComponent>(cameraEntities[0]);

	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f);
	cameraTransform->SetPositionVec3(cameraPosition);

	return dsr::DsrResult::Success("Setup Successful.");

	return dsr::DsrResult::Success("");
}
