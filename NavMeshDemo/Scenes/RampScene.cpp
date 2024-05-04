#include "RampScene.h"

#include <Data/Manipulation/StaticMeshExtensions.h>
#include "Data/Pathfinding/AStarStaticMeshPathfinder.h"

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"

#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/WireframeMeshComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/LineListComponent.h"


static constexpr auto StartIndex = 49;
static constexpr auto EndIndex = 30;

static constexpr auto StartIndexSubDivided = 578;
static constexpr auto EndIndexSubDivided = 320;

RampScene::RampScene(
	const std::shared_ptr<dsr::scenesystem::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
)	: m_sceneManager(sceneManager), m_device(device), m_blenderModelLoader(blenderModelLoader),
	m_sceneName("Ramp")
{
	m_sceneId = m_sceneManager->CreateNewScene(m_sceneName);

	m_mapEntity = m_sceneManager->CreateNewEntity();
	m_mapFaceNormalsEntity = m_sceneManager->CreateNewEntity();
	m_mapUpperSurfaceEntity = m_sceneManager->CreateNewEntity();
	m_mapUpperSurfaceSubDividedEntity = m_sceneManager->CreateNewEntity();
	m_pathMarkersEntity = m_sceneManager->CreateNewEntity();
	m_pathEntity = m_sceneManager->CreateNewEntity();
	m_pathSubDividedEntity = m_sceneManager->CreateNewEntity();
}

dsr::DsrResult RampScene::BuildScene()
{
	using namespace dsr;

	std::variant<std::vector<ModelConfiguration>, dsr_error> loadSceneDataResult = LoadSceneData();

	if (std::holds_alternative<dsr_error>(loadSceneDataResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadSceneDataResult);
		return DsrResult(error.what(), ERROR_LOADRAMPSCENE_MODELS);
	}

	const std::vector<ModelConfiguration>& models = std::get<std::vector<ModelConfiguration>>(loadSceneDataResult);

	RegisterMapModel(models[0]);
	RegisterMapUpperSurfaceModel(models[1]);
	RegisterMapUpperSurfaceSubDividedModel();
	RegisterStartEndMarkerEntities();
	RegisterPathEntity();
	RegisterSubDividedPathEntity();
	RegisterMapFaceNormalsEntity();

	return DsrResult::Success("Build RampScene Success.");
}

void RampScene::SetActive()
{
	m_sceneManager->SetActiveScene(m_sceneId);
}

std::variant<std::vector<dsr::ModelConfiguration>, dsr::dsr_error> RampScene::LoadSceneData()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<ModelConfiguration, dsr_error> loadMapResult = LoadMapModel();
	if (std::holds_alternative<dsr_error>(loadMapResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadMapResult);
		std::string errorMessage = "Error loading Map: ";
		errorMessage += error.what();
		return dsr_error(errorMessage, error.GetResult());
	}

	std::variant<ModelConfiguration, dsr_error> loadMapUpperSurfaceResult = LoadMapUpperSurfaceModel();
	if (std::holds_alternative<dsr_error>(loadMapUpperSurfaceResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadMapUpperSurfaceResult);
		std::string errorMessage = "Error loading Map Upper Surface: ";
		errorMessage += error.what();
		return dsr_error(errorMessage, error.GetResult());
	}

	std::vector<ModelConfiguration> configurations;
	configurations.push_back(std::get<ModelConfiguration>(loadMapResult));
	configurations.push_back(std::get<ModelConfiguration>(loadMapUpperSurfaceResult));

	return configurations;
}

std::variant<dsr::ModelConfiguration, dsr::dsr_error> RampScene::LoadMapModel()
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

std::variant<dsr::ModelConfiguration, dsr::dsr_error> RampScene::LoadMapUpperSurfaceModel()
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
	group.MaterialData.DiffuseColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_mapUpperSurfaceModel->MaterialGroups.push_back(group);

	return LoadWavefrontModelConfiguration(
		m_device,
		m_mapUpperSurfaceModel
	);
}

void RampScene::RegisterMapModel(const dsr::ModelConfiguration& map)
{
	using namespace dsr::ecs;

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_mapEntity, "map");
	m_sceneManager->AddComponent<TagComponent>(m_sceneId, m_mapEntity, "map");
	m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_mapEntity);

	std::shared_ptr<StaticMeshComponent> mesh = m_sceneManager->AddComponent<StaticMeshComponent>(m_sceneId, m_mapEntity);
	mesh->SetVertexBuffer(map.GetVertexBuffer());
	mesh->SetVertexGroups(map.GetVertexGroups());
}

void RampScene::RegisterMapUpperSurfaceModel(const dsr::ModelConfiguration& mapUpperSurface)
{
	using namespace dsr::ecs;

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_mapUpperSurfaceEntity, "map upper surface");
	std::shared_ptr<TransformComponent> transform = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_mapUpperSurfaceEntity);
	transform->SetPosition(DirectX::XMVectorSet(0.0f, 40.0f, 0.0f, 1.0f));

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(mapUpperSurface.GetVertexBuffer());
	mesh->SetVertexGroups(mapUpperSurface.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_sceneManager->AddComponent<WireframeMeshComponent>(m_sceneId, m_mapUpperSurfaceEntity);
	wireframe->SetMesh(mesh);
}

void RampScene::RegisterMapUpperSurfaceSubDividedModel()
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
	group.MaterialData.DiffuseColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
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

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_mapUpperSurfaceSubDividedEntity, "Subdivided Upper Surface");
	std::shared_ptr<TransformComponent> transform = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_mapUpperSurfaceSubDividedEntity);
	transform->SetPosition(DirectX::XMVectorSet(0.0f, 80.0f, 0.0f, 1.0f));

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(model.GetVertexBuffer());
	mesh->SetVertexGroups(model.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_sceneManager->AddComponent<WireframeMeshComponent>(m_sceneId, m_mapUpperSurfaceSubDividedEntity);
	wireframe->SetMesh(mesh);
}

void RampScene::RegisterStartEndMarkerEntities()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::directX;
	using namespace dsr::ecs;

	using namespace DirectX;

	std::shared_ptr<TransformComponent> surfaceTransform = m_sceneManager->GetComponentFrom<TransformComponent>(m_sceneId, m_mapUpperSurfaceEntity);
	XMFLOAT3 surfacePosition = surfaceTransform->GetPositionVec3();
	XMMATRIX surfaceTransformationMatrix = XMMatrixTranslation(surfacePosition.x, surfacePosition.y, surfacePosition.z);

	const std::vector<Vertex3FP2FTx3FN>& vertexBuffer = m_mapUpperSurfaceModel->Mesh->GetVertexBuffer();

	std::vector<float> vertexData;
	AddMarkerLine(vertexBuffer[StartIndex], surfaceTransformationMatrix, vertexData);
	AddMarkerLine(vertexBuffer[EndIndex], surfaceTransformationMatrix, vertexData);

	std::shared_ptr<TransformComponent> surfaceSubDividedTransform = m_sceneManager->GetComponentFrom<TransformComponent>(m_sceneId, m_mapUpperSurfaceSubDividedEntity);
	XMFLOAT3 surfaceSubDividedPosition = surfaceSubDividedTransform->GetPositionVec3();
	XMMATRIX surfaceSubDividedTransformationMatrix = XMMatrixTranslation(surfaceSubDividedPosition.x, surfaceSubDividedPosition.y, surfaceSubDividedPosition.z);

	const std::vector<Vertex3FP2FTx3FN>& vertexBufferSubDivided = m_mapUpperSurfaceSubDividedModel->Mesh->GetVertexBuffer();
	AddMarkerLine(vertexBufferSubDivided[StartIndexSubDivided], surfaceSubDividedTransformationMatrix, vertexData);
	AddMarkerLine(vertexBufferSubDivided[EndIndexSubDivided], surfaceSubDividedTransformationMatrix, vertexData);

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

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_sceneManager->AddComponent<dsr::ecs::LineListComponent>(m_sceneId, m_pathMarkersEntity);

	lines->SetVertexCount(vertexData.size() / 7);
	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
	lines->SetVertexShaderInputLayout(inputLayout);
}

void RampScene::RegisterPathEntity()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;
	using namespace dsr::data::pathfinding;
	using namespace dsr::directX;
	using namespace dsr::ecs;

	using namespace DirectX;

	StaticMesh<Vertex3F> distinctMesh = FilterDistinct(*m_mapUpperSurfaceModel->Mesh);
	AStarStaticMeshPathfinder pathfinder(distinctMesh);
	std::vector<uint32_t> path = pathfinder.SearchSequential(StartIndex, EndIndex);
	std::vector<float> vertexData = GetLinePath(distinctMesh, path, {1.0f, 0.0f, 1.0f, 1.0f});

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

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_sceneManager->AddComponent<dsr::ecs::LineListComponent>(m_sceneId, m_pathEntity);
	std::shared_ptr<TransformComponent> transform = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_pathEntity);
	std::shared_ptr<TransformComponent> surfaceTransform = m_sceneManager->GetComponentFrom<TransformComponent>(m_sceneId, m_mapUpperSurfaceEntity);
	XMFLOAT3 surfacePosition = surfaceTransform->GetPositionVec3();
	++surfacePosition.y;
	transform->SetPositionVec3(surfacePosition);

	lines->SetVertexCount(vertexData.size() / 7);
	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
	lines->SetVertexShaderInputLayout(inputLayout);
}

void RampScene::RegisterSubDividedPathEntity()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;
	using namespace dsr::data::pathfinding;
	using namespace dsr::directX;
	using namespace dsr::ecs;

	using namespace DirectX;

	StaticMesh<Vertex3F> distincSubDivision = FilterDistinct(*m_mapUpperSurfaceSubDividedModel->Mesh);
	AStarStaticMeshPathfinder pathfinderSubDivision(distincSubDivision);

	/*std::chrono::time_point prev = std::chrono::high_resolution_clock::now();*/
	std::vector<uint32_t> pathSubDivided = pathfinderSubDivision.SearchSequential(StartIndexSubDivided, EndIndexSubDivided);
	/*std::chrono::time_point current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration = current - prev;*/

	std::vector<float> vertexData = GetLinePath(distincSubDivision, pathSubDivided, { 1.0f, 0.0f, 1.0f, 1.0f });
	std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(m_device, vertexData);
	if (std::holds_alternative<dsr_error>(createVertexBuffer))
	{
		const dsr_error& err = std::get<dsr_error>(createVertexBuffer);
		std::cout << "error creatingVertexBuffer for Subdivided path LineData: " << err.what() << std::endl;
		return;
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_sceneManager->AddComponent<dsr::ecs::LineListComponent>(m_sceneId, m_pathSubDividedEntity);
	std::shared_ptr<TransformComponent> transform = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_pathSubDividedEntity);
	std::shared_ptr<TransformComponent> surfaceTransform = m_sceneManager->GetComponentFrom<TransformComponent>(m_sceneId, m_mapUpperSurfaceSubDividedEntity);
	XMFLOAT3 surfacePosition = surfaceTransform->GetPositionVec3();
	++surfacePosition.y;
	transform->SetPositionVec3(surfacePosition);

	lines->SetVertexCount(vertexData.size() / 7);
	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
	lines->SetVertexShaderInputLayout(inputLayout);
}

void RampScene::RegisterMapFaceNormalsEntity()
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

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_sceneManager->AddComponent<dsr::ecs::LineListComponent>(m_sceneId, m_mapFaceNormalsEntity);
	lines->SetVertexCount(mapFaces.size() * 2);
	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
	lines->SetVertexShaderInputLayout(inputLayout);
}

void RampScene::AddMarkerLine(const dsr::data::Vertex3FP2FTx3FN& vertex, const DirectX::XMMATRIX& transform, std::vector<float>& vertexBufferData)
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
