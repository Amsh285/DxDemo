#include "BridgeScene.h"

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"
#include "Data/Manipulation/StaticMeshExtensions.h"
#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"

BridgeScene::BridgeScene(
	const std::shared_ptr<dsr::scenesystem::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader)
	: m_sceneManager(sceneManager), m_device(device), m_blenderModelLoader(blenderModelLoader),
	m_sceneName("Bridge")
{
	m_sceneId = m_sceneManager->CreateNewScene(m_sceneName);

	m_mapEntity = m_sceneManager->CreateNewEntity();
}

dsr::DsrResult BridgeScene::BuildScene()
{
	using namespace dsr;

	std::variant<std::vector<ModelConfiguration>, dsr_error> loadSceneDataResult = LoadSceneData();

	if (std::holds_alternative<dsr_error>(loadSceneDataResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadSceneDataResult);
		return DsrResult(error.what(), ERROR_LOADBRIDGESCENE_MODELS);
	}

	const std::vector<ModelConfiguration>& models = std::get<std::vector<ModelConfiguration>>(loadSceneDataResult);

	RegisterMapModel(models[0]);

	return dsr::DsrResult::Success("Build BridgeScene Success");
}

std::variant<std::vector<dsr::ModelConfiguration>, dsr::dsr_error> BridgeScene::LoadSceneData()
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

std::variant<dsr::ModelConfiguration, dsr::dsr_error> BridgeScene::LoadMapModel()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<std::shared_ptr<WavefrontModel>, dsr_error> loadMapWfResult = LoadWavefrontModel(m_blenderModelLoader,
		"Assets/",
		"Bridges.wf",
		"Bridges.mtl");

	if (std::holds_alternative<dsr_error>(loadMapWfResult))
		return std::get<dsr_error>(loadMapWfResult);

	m_mapModel = std::get<std::shared_ptr<WavefrontModel>>(loadMapWfResult);

	return LoadWavefrontModelConfiguration(
		m_device,
		m_mapModel
	);
}

std::variant<dsr::ModelConfiguration, dsr::dsr_error> BridgeScene::LoadMapUpperSurfaceModel()
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

void BridgeScene::RegisterMapModel(const dsr::ModelConfiguration& map)
{
	using namespace dsr::ecs;

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_mapEntity, "map");
	m_sceneManager->AddComponent<TagComponent>(m_sceneId, m_mapEntity, "map");
	m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_mapEntity);

	std::shared_ptr<StaticMeshComponent> mesh = m_sceneManager->AddComponent<StaticMeshComponent>(m_sceneId, m_mapEntity);
	mesh->SetVertexBuffer(map.GetVertexBuffer());
	mesh->SetVertexGroups(map.GetVertexGroups());
}
