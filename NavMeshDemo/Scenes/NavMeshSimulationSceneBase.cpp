#include "NavMeshSimulationSceneBase.h"

#include <Data/Manipulation/StaticMeshExtensions.h>

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"

#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"

NavMeshSimulationSceneBase::NavMeshSimulationSceneBase(
	const std::string& sceneName,
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
) : m_sceneName(sceneName), m_sceneManager(sceneManager), m_device(device), m_blenderModelLoader(blenderModelLoader)
{
	m_sceneSettings.BaseMeshFileName = m_sceneName + ".wf";
	m_sceneSettings.BaseMeshMaterialFileName = m_sceneName + ".mtl";

	m_sceneId = m_sceneManager->CreateNewScene(m_sceneName);

	m_baseMeshEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceEntity = m_sceneManager->CreateNewEntity();
}

void NavMeshSimulationSceneBase::SetActive()
{
	m_sceneManager->SetActiveScene(m_sceneId);
}

dsr::DsrResult NavMeshSimulationSceneBase::BuildScene()
{
	using namespace dsr;

	DsrResult loadSceneDataResult = LoadSceneData();

	if (loadSceneDataResult.GetResultStatusCode() != RESULT_SUCCESS)
		return loadSceneDataResult;



	

	return DsrResult::Success("Build Scene: " + m_sceneName + " Success.");
}

dsr::DsrResult NavMeshSimulationSceneBase::LoadSceneData()
{
	using namespace dsr;

	DsrResult loadBaseMeshResult = LoadBaseMesh();

	if (loadBaseMeshResult.GetResultStatusCode() != RESULT_SUCCESS)
		return loadBaseMeshResult;

	LoadUpperSurface();

	DsrResult registerBaseMeshResult = RegisterBaseMesh();

	if (registerBaseMeshResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerBaseMeshResult;

	return DsrResult::Success("Load SceneData: " + m_sceneName + " Success.");
}

dsr::DsrResult NavMeshSimulationSceneBase::LoadBaseMesh()
{  
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<std::shared_ptr<WavefrontModel>, dsr_error> loadBaseMeshResult = LoadWavefrontModel(m_blenderModelLoader,
		m_sceneSettings.AssetBaseDirectory,
		m_sceneSettings.BaseMeshFileName,
		m_sceneSettings.BaseMeshMaterialFileName
	);

	if (std::holds_alternative<dsr_error>(loadBaseMeshResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadBaseMeshResult);
		std::string errorMessage = "Error loading Basemesh: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_LOAD_BASEMESH);
	}
	
	m_baseMesh = std::get<std::shared_ptr<WavefrontModel>>(loadBaseMeshResult);

	return DsrResult::Success("Load Basemesh Success.");
}

void NavMeshSimulationSceneBase::LoadUpperSurface()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;

	using namespace DirectX;

	std::shared_ptr<StaticMesh<Vertex3FP2FTx3FN>> filteredMesh = FilterByNormalAngles(
		m_baseMesh->Mesh,
		XMConvertToRadians(45.0f)
	);

	m_upperSurface = std::make_shared<WavefrontModel>();
	m_upperSurface->Mesh = filteredMesh;
	m_upperSurface->Mesh->RefreshHitTestCache();

	WavefrontModelMaterialGroup group;
	group.IndexCount = m_upperSurface->Mesh->GetIndexBuffer().size();
	group.StartIndexLocation = 0;
	group.MaterialName = "mat";
	group.MaterialData.SpecularColor = XMFLOAT3(0.8f, 0.8f, 0.8f);
	group.MaterialData.DiffuseColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_upperSurface->MaterialGroups.push_back(group);
}

dsr::DsrResult NavMeshSimulationSceneBase::RegisterBaseMesh()
{
	using namespace dsr;

	using namespace dsr::directX::rendering;

	using namespace dsr::ecs;

	std::variant<ModelConfiguration, dsr_error> loadConfigurationResult = LoadWavefrontModelConfiguration(
		m_device,
		m_baseMesh
	);

	if (std::holds_alternative<dsr_error>(loadConfigurationResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadConfigurationResult);
		std::string errorMessage = "Error setup Model-configuration for Basemesh: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_REGISTER_BASEMESH_SETUPMODEL);
	}

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_baseMeshEntity, "map");
	m_sceneManager->AddComponent<TagComponent>(m_sceneId, m_baseMeshEntity, "map");
	m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_baseMeshEntity);

	ModelConfiguration config = std::get<ModelConfiguration>(loadConfigurationResult);

	std::shared_ptr<StaticMeshComponent> mesh = m_sceneManager->AddComponent<StaticMeshComponent>(m_sceneId, m_baseMeshEntity);
	mesh->SetVertexBuffer(config.GetVertexBuffer());
	mesh->SetVertexGroups(config.GetVertexGroups());

	return DsrResult::Success("Register Basemesh success.");
}
