#include "NavMeshDemoApplication.h"

static constexpr auto StartIndex = 49;
static constexpr auto EndIndex = 30;

static constexpr auto StartIndexSubDivided = 578;
static constexpr auto EndIndexSubDivided = 320;

NavMeshDemoApplication::NavMeshDemoApplication()
	: DsrApplication(L"Nav Mesh Demo", 100, 100, 1280, 768)
{
	using namespace dsr::ecs;

	m_mapEntity = EcsManager::CreateNewEntity();
	m_mapFaceNormalsEntity = EcsManager::CreateNewEntity();
	m_mapUpperSurfaceEntity = EcsManager::CreateNewEntity();
	m_mapUpperSurfaceSubDividedEntity = EcsManager::CreateNewEntity();
	m_lineEntity = EcsManager::CreateNewEntity();
	m_pathMarkersEntity = EcsManager::CreateNewEntity();
	m_pathEntity = EcsManager::CreateNewEntity();
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

	m_ecsManager->RegisterComponent<NameComponent>(m_mapUpperSurfaceEntity, "map upper surface");
	std::shared_ptr<TransformComponent> transform = m_ecsManager->RegisterComponent<TransformComponent>(m_mapUpperSurfaceEntity);
	transform->SetPosition(DirectX::XMVectorSet(0.0f, 40.0f, 0.0f, 1.0f));

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(mapUpperSurface.GetVertexBuffer());
	mesh->SetVertexGroups(mapUpperSurface.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_ecsManager->RegisterComponent<WireframeMeshComponent>(m_mapUpperSurfaceEntity);
	wireframe->SetMesh(mesh);
}

void NavMeshDemoApplication::RegisterMapUpperSurfaceSubDividedModel()
{
	using namespace dsr;

	using namespace dsr::data;
	using namespace dsr::data::manipulation;

	using namespace dsr::directX::rendering;

	using namespace dsr::ecs;

	using namespace DirectX;

	// fix that... not really effective
	m_mapUpperSurfaceSubDividedModel = std::make_shared<WavefrontModel>();
	m_mapUpperSurfaceSubDividedModel->Mesh = SubDivide(m_mapUpperSurfaceModel->Mesh);
	m_mapUpperSurfaceSubDividedModel->Mesh = SubDivide(m_mapUpperSurfaceSubDividedModel->Mesh);
	m_mapUpperSurfaceSubDividedModel->Mesh = SubDivide(m_mapUpperSurfaceSubDividedModel->Mesh);
	m_mapUpperSurfaceSubDividedModel->Mesh = SubDivide(m_mapUpperSurfaceSubDividedModel->Mesh);

	WavefrontModelMaterialGroup group;
	group.IndexCount = m_mapUpperSurfaceSubDividedModel->Mesh->GetIndexBuffer().size();
	group.StartIndexLocation = 0;
	group.MaterialName = "mat";
	group.MaterialData.SpecularColor = XMFLOAT3(0.8f, 0.8f, 0.8f);
	group.MaterialData.DiffuseColor = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_mapUpperSurfaceSubDividedModel->MaterialGroups.push_back(group);


	std::variant<ModelConfiguration, dsr_error> loadSurfaceResult = LoadWavefrontModelConfiguration(
		m_device,
		m_mapUpperSurfaceSubDividedModel
	);

	if (std::holds_alternative<dsr_error>(loadSurfaceResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadSurfaceResult);
		std::cout << "Failed to load Subdivided Upper Surface: " << error.what() << std::endl;
		return;
	}

	ModelConfiguration model = std::get<ModelConfiguration>(loadSurfaceResult);
	
	m_ecsManager->RegisterComponent<NameComponent>(m_mapUpperSurfaceSubDividedEntity, "Subdivided Upper Surface");
	std::shared_ptr<TransformComponent> transform = m_ecsManager->RegisterComponent<TransformComponent>(m_mapUpperSurfaceSubDividedEntity);
	transform->SetPosition(DirectX::XMVectorSet(0.0f, 80.0f, 0.0f, 1.0f));

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(model.GetVertexBuffer());
	mesh->SetVertexGroups(model.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_ecsManager->RegisterComponent<WireframeMeshComponent>(m_mapUpperSurfaceSubDividedEntity);
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

void NavMeshDemoApplication::RegisterStartEndMarkerEntities()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::directX;
	using namespace dsr::ecs;

	using namespace DirectX;

	std::shared_ptr<TransformComponent> surfaceTransform = m_ecsManager->GetComponentFrom<TransformComponent>(m_mapUpperSurfaceEntity);
	XMFLOAT3 surfacePosition = surfaceTransform->GetPositionVec3();
	XMMATRIX surfaceTransformationMatrix = XMMatrixTranslation(surfacePosition.x, surfacePosition.y, surfacePosition.z);

	const std::vector<Vertex3FP2FTx3FN>& vertexBuffer = m_mapUpperSurfaceModel->Mesh->GetVertexBuffer();

	std::vector<float> vertexData;
	AddMarkerLine(vertexBuffer[StartIndex], surfaceTransformationMatrix, vertexData);
	AddMarkerLine(vertexBuffer[EndIndex], surfaceTransformationMatrix, vertexData);

	std::shared_ptr<TransformComponent> surfaceSubDividedTransform = m_ecsManager->GetComponentFrom<TransformComponent>(m_mapUpperSurfaceSubDividedEntity);
	XMFLOAT3 surfaceSubDividedPosition = surfaceSubDividedTransform->GetPositionVec3();
	XMMATRIX surfaceSubDividedTransformationMatrix = XMMatrixTranslation(surfaceSubDividedPosition.x, surfaceSubDividedPosition.y, surfaceSubDividedPosition.z);

	const std::vector<Vertex3FP2FTx3FN>& vertexBufferSubDivided = m_mapUpperSurfaceSubDividedModel->Mesh->GetVertexBuffer();
	AddMarkerLine(vertexBufferSubDivided[StartIndexSubDivided], surfaceSubDividedTransformationMatrix, vertexData);
	AddMarkerLine(vertexBufferSubDivided[EndIndexSubDivided], surfaceSubDividedTransformationMatrix, vertexData);

	/*for (size_t i = 0; i < vertexBufferSubDivided.size(); i++)
	{
		std::cout << i << " " << vertexBufferSubDivided[i].Position.x << " " << vertexBufferSubDivided[i].Position.y << " " << vertexBufferSubDivided[i].Position.z << " " << std::endl;
	}*/


	std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(m_device, vertexData);
	if (std::holds_alternative<dsr_error>(createVertexBuffer))
	{
		const dsr_error& err = std::get<dsr_error>(createVertexBuffer);
		std::cout << "error creatingVertexBuffer for pathmarker LineData: " << err.what() << std::endl;
		return;
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_ecsManager->RegisterComponent<dsr::ecs::LineListComponent>(m_pathMarkersEntity);

	lines->SetVertexCount(vertexData.size() / 7);
	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
	lines->SetVertexShaderInputLayout(inputLayout);
}

void NavMeshDemoApplication::RegisterPathEntity()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;
	using namespace dsr::data::pathfinding;
	using namespace dsr::directX;
	using namespace dsr::ecs;

	using namespace DirectX;

	//auto adList = m_mapUpperSurfaceModel->Mesh->GetAdjacencyList();

	//for (auto pair : adList)
	//{
	//	std::cout << pair.first;

	//	for (auto item : pair.second)
	//	{
	//		std::cout << " " << item << " ";
	//	}

	//	std::cout << std::endl;
	//}

	StaticMesh<Vertex3F> distinctMesh = FilterDistinct(*m_mapUpperSurfaceModel->Mesh);
	/*for (auto pair : distinctMesh.GetAdjacencyList())
	{
		std::cout << pair.first;

		for (auto item : pair.second)
		{
			std::cout << " " << item << " ";
		}

		std::cout << std::endl;
	}*/

	const std::vector<Vertex3F>& vertexBuffer = distinctMesh.GetVertexBuffer();

	AStarStaticMeshPathfinder pathfinder(distinctMesh);
	std::vector<uint32_t> path = pathfinder.SearchIndexPath(StartIndex, EndIndex);

	std::shared_ptr<TransformComponent> surfaceTransform = m_ecsManager->GetComponentFrom<TransformComponent>(m_mapUpperSurfaceEntity);
	XMFLOAT3 surfacePosition = surfaceTransform->GetPositionVec3();
	XMMATRIX surfaceTransformationMatrix = XMMatrixTranslation(surfacePosition.x, surfacePosition.y + 1.0f, surfacePosition.z);

	std::vector<float> vertexData;

	for (size_t i = 0; i < path.size() - 1; i++)
	{
		uint32_t currentIndex = path[i];
		uint32_t nextIndex = path[i + 1];

		XMVECTOR v0 = XMVector3Transform(XMLoadFloat3(&vertexBuffer[currentIndex].Position), surfaceTransformationMatrix);
		XMVECTOR v1 = XMVector3Transform(XMLoadFloat3(&vertexBuffer[nextIndex].Position), surfaceTransformationMatrix);

		vertexData.push_back(XMVectorGetX(v0));
		vertexData.push_back(XMVectorGetY(v0));
		vertexData.push_back(XMVectorGetZ(v0));

		vertexData.push_back(1.0f);
		vertexData.push_back(0.0f);
		vertexData.push_back(1.0f);
		vertexData.push_back(1.0f);

		vertexData.push_back(XMVectorGetX(v1));
		vertexData.push_back(XMVectorGetY(v1));
		vertexData.push_back(XMVectorGetZ(v1));

		vertexData.push_back(1.0f);
		vertexData.push_back(0.0f);
		vertexData.push_back(1.0f);
		vertexData.push_back(1.0f);
	}

	std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(m_device, vertexData);
	if (std::holds_alternative<dsr_error>(createVertexBuffer))
	{
		const dsr_error& err = std::get<dsr_error>(createVertexBuffer);
		std::cout << "error creatingVertexBuffer for path LineData: " << err.what() << std::endl;
		return;
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_ecsManager->RegisterComponent<dsr::ecs::LineListComponent>(m_pathEntity);

	lines->SetVertexCount(vertexData.size() / 7);
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

void NavMeshDemoApplication::AddMarkerLine(const dsr::data::Vertex3FP2FTx3FN& vertex, const DirectX::XMMATRIX& transform, std::vector<float>& vertexBufferData)
{
	using namespace dsr::data;

	using namespace DirectX;

	XMVECTOR up = XMVectorAdd(XMLoadFloat3(&vertex.Position), XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f));
	XMVECTOR down = XMVectorAdd(XMLoadFloat3(&vertex.Position), XMVectorSet(0.0f, -3.0f, 0.0f, 0.0f));
	up = XMVector3Transform(up, transform);
	down = XMVector3Transform(down, transform);

	vertexBufferData.push_back(XMVectorGetX(up));
	vertexBufferData.push_back(XMVectorGetY(up));
	vertexBufferData.push_back(XMVectorGetZ(up));

	vertexBufferData.push_back(1.0f);
	vertexBufferData.push_back(0.0f);
	vertexBufferData.push_back(1.0f);
	vertexBufferData.push_back(1.0f);

	vertexBufferData.push_back(XMVectorGetX(down));
	vertexBufferData.push_back(XMVectorGetY(down));
	vertexBufferData.push_back(XMVectorGetZ(down));

	vertexBufferData.push_back(1.0f);
	vertexBufferData.push_back(0.0f);
	vertexBufferData.push_back(1.0f);
	vertexBufferData.push_back(1.0f);
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
	RegisterMapUpperSurfaceSubDividedModel();
	RegisterLineEntity();
	RegisterStartEndMarkerEntities();
	RegisterPathEntity();
	RegisterMapFaceNormalsEntity();
	RegisterCameraController();

	std::vector<dsr::ecs::Entity> cameraEntities = m_ecsManager->FindEntitiesByTag("Camera");
	std::shared_ptr<dsr::ecs::TransformComponent> cameraTransform = m_ecsManager->GetComponentFrom<dsr::ecs::TransformComponent>(cameraEntities[0]);

	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f);
	cameraTransform->SetPositionVec3(cameraPosition);

	return dsr::DsrResult::Success("Setup Successful.");
}
