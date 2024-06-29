#include "NavMeshSimulationSceneBase.h"

#include <Data/Manipulation/StaticMeshExtensions.h>

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"

#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/WireframeMeshComponent.h"

NavMeshSimulationSceneBase::NavMeshSimulationSceneBase(
	const std::string& sceneName,
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
) : m_sceneName(sceneName), m_sceneId(sceneManager->CreateNewScene(sceneName)),
m_sceneManager(sceneManager), m_device(device), m_blenderModelLoader(blenderModelLoader)
{
	m_markers = std::make_unique<NavMeshSimulationSceneMarkers>(m_sceneId, sceneManager, device);
	m_paths = std::make_unique<NavMeshSimulationScenePaths>(m_sceneId, sceneManager, device);

	m_sceneSettings.BaseMeshFileName = m_sceneName + ".wf";
	m_sceneSettings.BaseMeshMaterialFileName = m_sceneName + ".mtl";

	m_baseMeshEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceSubDivisionEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceBarycentricSubDivisionEntity = m_sceneManager->CreateNewEntity();
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

void NavMeshSimulationSceneBase::OnScreenClick(const EditorScreenClickEvent& screenClickEvent)
{
	using namespace dsr;

	using namespace dsr::data;
	using namespace dsr::data::manipulation;
	using namespace dsr::events;
	using namespace dsr::inputdevices;

	using namespace dsr::directX;

	using namespace dsr::ecs;
	using namespace dsr::scene;

	using namespace DirectX;

	std::shared_ptr<Camera> activeCamera = Camera::GetActiveCamera();

	if (!activeCamera)
		return;

	MousePosition position = screenClickEvent.GetPosition();
	Screen screen = screenClickEvent.GetScreen();

	XMVECTOR rayOrigin = activeCamera->GetTransform()->GetPosition();
	XMVECTOR rayDirection = activeCamera->ScreenToWorld(
		position.X, position.Y,
		screen.GetClientWidth(), screen.GetClientHeight()
	);

	std::shared_ptr<TransformComponent> transform = m_sceneManager->GetComponentFrom<TransformComponent>(m_sceneId, m_baseMeshEntity);
	XMMATRIX model = transform->GetModelMatrix();
	XMVECTOR determinant = XMMatrixDeterminant(model);
	XMMATRIX inverseModel = XMMatrixInverse(&determinant, model);

	std::vector<RaycastMeshHit> hits = GetMeshIntersections(
		m_upperSurface->Mesh,
		XMVector4Transform(rayOrigin, inverseModel),
		XMVector4Transform(rayDirection, inverseModel)
	);

	//Debug::DrawRay(rayOrigin, rayDirection, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0.2f, 100.0f, std::chrono::seconds(5));

	if (hits.size() > 0)
	{
		m_markers->SetMarkerPositions(
			screenClickEvent.GetPathSelectionMode(),
			XMVectorSet(hits[0].Intersection.x, hits[0].Intersection.y, hits[0].Intersection.z, 1.0f)
		);

		DsrResult setPathsResult = m_paths->SetPaths(
			m_markers->GetStartPositionLocal(),
			m_markers->GetFinishPositionLocal()
		);

		if (setPathsResult.GetResultStatusCode() != RESULT_SUCCESS)
			std::cout << setPathsResult.GetResultMessage() << std::endl;
	}
}

dsr::DsrResult NavMeshSimulationSceneBase::LoadSceneData()
{
	using namespace dsr;

	DsrResult loadBaseMeshResult = LoadBaseMesh();

	if (loadBaseMeshResult.GetResultStatusCode() != RESULT_SUCCESS)
		return loadBaseMeshResult;

	LoadUpperSurface();
	LoadUpperSurfaceSubDivision();
	LoadUpperSurfaceBarycentricSubDivision();

	DsrResult registerBaseMeshResult = RegisterBaseMesh();
	if (registerBaseMeshResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerBaseMeshResult;

	DsrResult registerUpperSurfaceResult = RegisterUpperSurface();
	if (registerUpperSurfaceResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerUpperSurfaceResult;

	DsrResult registerUpperSurfaceSubDivisionResult = RegisterUpperSurfaceSubDivision();
	if (registerUpperSurfaceSubDivisionResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerUpperSurfaceSubDivisionResult;

	DsrResult registerUpperSurfaceBarycentricSubDivisionResult = RegisterUpperSurfaceBarycentricSubDivision();
	if (registerUpperSurfaceBarycentricSubDivisionResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerUpperSurfaceBarycentricSubDivisionResult;

	DsrResult setupMarkersResult = m_markers->SetupMarkers(m_sceneSettings, m_upperSurface);
	if (setupMarkersResult.GetResultStatusCode() != RESULT_SUCCESS)
		return setupMarkersResult;

	m_paths->Setup(
		m_sceneSettings,
		m_upperSurface,
		m_upperSurfaceSubDivision,
		m_upperSurfaceBarycentricSubDivision
	);

	m_paths->SetPaths(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal());

	return DsrResult::Success("Load SceneData: " + m_sceneName + " Success.");
}

dsr::DsrResult NavMeshSimulationSceneBase::LoadBaseMesh()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	using namespace dsr::data;
	using namespace dsr::data::manipulation;

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
	m_baseMesh->Mesh = std::make_shared<StaticMesh<Vertex3FP2FTx3FN>>(RemoveDegenerateTriangles(*m_baseMesh->Mesh));
	m_baseMesh->MaterialGroups[0].IndexCount = m_baseMesh->Mesh->GetIndexBuffer().size();

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

void NavMeshSimulationSceneBase::LoadUpperSurfaceSubDivision()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;

	using namespace DirectX;

	m_upperSurfaceSubDivision = std::make_shared<WavefrontModel>();
	m_upperSurfaceSubDivision->Mesh = std::make_shared<StaticMesh<Vertex3FP2FTx3FN>>(SubDivide(*m_upperSurface->Mesh));

	WavefrontModelMaterialGroup group;
	group.IndexCount = m_upperSurfaceSubDivision->Mesh->GetIndexBuffer().size();
	group.StartIndexLocation = 0;
	group.MaterialName = "mat";
	group.MaterialData.SpecularColor = XMFLOAT3(0.8f, 0.8f, 0.8f);
	group.MaterialData.DiffuseColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_upperSurfaceSubDivision->MaterialGroups.push_back(group);
}

void NavMeshSimulationSceneBase::LoadUpperSurfaceBarycentricSubDivision()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;

	using namespace DirectX;

	m_upperSurfaceBarycentricSubDivision = std::make_shared<dsr::WavefrontModel>();
	m_upperSurfaceBarycentricSubDivision->Mesh = std::make_shared<StaticMesh<Vertex3FP2FTx3FN>>(SubDivideBarycentric(*m_upperSurface->Mesh));

	WavefrontModelMaterialGroup group;
	group.IndexCount = m_upperSurfaceBarycentricSubDivision->Mesh->GetIndexBuffer().size();
	group.StartIndexLocation = 0;
	group.MaterialName = "mat";
	group.MaterialData.SpecularColor = XMFLOAT3(0.8f, 0.8f, 0.8f);
	group.MaterialData.DiffuseColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_upperSurfaceBarycentricSubDivision->MaterialGroups.push_back(group);
}

dsr::DsrResult NavMeshSimulationSceneBase::RegisterBaseMesh()
{
	using namespace dsr;

	using namespace dsr::directX::rendering;

	using namespace dsr::ecs;

	using namespace DirectX;

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

	std::shared_ptr<TransformComponent> transformComponent = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_baseMeshEntity);

	XMVECTOR scale, rotationQuaternion, transform;
	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, m_sceneSettings.BaseMeshModel);
	transformComponent->SetScale(scale);
	transformComponent->SetRotation(rotationQuaternion);
	transformComponent->SetPosition(transform);

	ModelConfiguration config = std::get<ModelConfiguration>(loadConfigurationResult);

	std::shared_ptr<StaticMeshComponent> mesh = m_sceneManager->AddComponent<StaticMeshComponent>(m_sceneId, m_baseMeshEntity);
	mesh->SetVertexBuffer(config.GetVertexBuffer());
	mesh->SetVertexGroups(config.GetVertexGroups());

	return DsrResult::Success("Register Basemesh success.");
}

dsr::DsrResult NavMeshSimulationSceneBase::RegisterUpperSurface()
{
	using namespace dsr;

	using namespace dsr::directX::rendering;

	using namespace dsr::ecs;

	using namespace DirectX;

	std::variant<ModelConfiguration, dsr_error> loadUpperSurfaceResult = LoadWavefrontModelConfiguration(
		m_device,
		m_upperSurface
	);

	if (std::holds_alternative<dsr_error>(loadUpperSurfaceResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadUpperSurfaceResult);
		std::string errorMessage = "Error setup Model-configuration for Upper surface: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_REGISTER_UPPERSURFACE_SETUPMODEL);
	}

	ModelConfiguration config = std::get<ModelConfiguration>(loadUpperSurfaceResult);

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_upperSurfaceEntity, "map upper surface");
	std::shared_ptr<TransformComponent> transformComponent = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_upperSurfaceEntity);

	XMVECTOR scale, rotationQuaternion, transform;
	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, m_sceneSettings.UpperSurfaceModel);
	transformComponent->SetScale(scale);
	transformComponent->SetRotation(rotationQuaternion);
	transformComponent->SetPosition(transform);

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(config.GetVertexBuffer());
	mesh->SetVertexGroups(config.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_sceneManager->AddComponent<WireframeMeshComponent>(m_sceneId, m_upperSurfaceEntity);
	wireframe->SetMesh(mesh);

	return DsrResult::Success("Register Upper surface success");
}

dsr::DsrResult NavMeshSimulationSceneBase::RegisterUpperSurfaceSubDivision()
{
	using namespace dsr;

	using namespace dsr::directX::rendering;

	using namespace dsr::ecs;

	using namespace DirectX;

	std::variant<ModelConfiguration, dsr_error> loadSurfaceResult = LoadWavefrontModelConfiguration(
		m_device,
		m_upperSurfaceSubDivision
	);

	if (std::holds_alternative<dsr_error>(loadSurfaceResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadSurfaceResult);
		std::string errorMessage = "Error setup Model-configuration for subdivided Upper surface: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_REGISTER_SUBDIVIDED_UPPERSURFACE_SETUPMODEL);
	}

	ModelConfiguration config = std::get<ModelConfiguration>(loadSurfaceResult);

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_upperSurfaceSubDivisionEntity, "Subdivided Upper Surface");
	std::shared_ptr<TransformComponent> transformComponent = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_upperSurfaceSubDivisionEntity);

	XMVECTOR scale, rotationQuaternion, transform;
	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, m_sceneSettings.UpperSurfaceSubDivisonModel);
	transformComponent->SetScale(scale);
	transformComponent->SetRotation(rotationQuaternion);
	transformComponent->SetPosition(transform);

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(config.GetVertexBuffer());
	mesh->SetVertexGroups(config.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_sceneManager->AddComponent<WireframeMeshComponent>(m_sceneId, m_upperSurfaceSubDivisionEntity);
	wireframe->SetMesh(mesh);

	return DsrResult::Success("Register Upper surface subdivision success.");
}

dsr::DsrResult NavMeshSimulationSceneBase::RegisterUpperSurfaceBarycentricSubDivision()
{
	using namespace dsr;

	using namespace dsr::directX::rendering;

	using namespace dsr::ecs;

	using namespace DirectX;

	std::variant<ModelConfiguration, dsr_error> loadSurfaceResult = LoadWavefrontModelConfiguration(
		m_device,
		m_upperSurfaceBarycentricSubDivision
	);

	if (std::holds_alternative<dsr_error>(loadSurfaceResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadSurfaceResult);
		std::string errorMessage = "Error setup Model-configuration for barycentric subdivided Upper surface: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_REGISTER_BARYCENTRICSUBDIVIDED_UPPERSURFACE_SETUPMODEL);
	}

	ModelConfiguration config = std::get<ModelConfiguration>(loadSurfaceResult);

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, m_upperSurfaceSubDivisionEntity, "Subdivided Upper Surface");
	std::shared_ptr<TransformComponent> transformComponent = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, m_upperSurfaceBarycentricSubDivisionEntity);

	XMVECTOR scale, rotationQuaternion, transform;
	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, m_sceneSettings.UpperSurfaceBarycentricSubDivisionModel);
	transformComponent->SetScale(scale);
	transformComponent->SetRotation(rotationQuaternion);
	transformComponent->SetPosition(transform);

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(config.GetVertexBuffer());
	mesh->SetVertexGroups(config.GetVertexGroups());

	std::shared_ptr<WireframeMeshComponent> wireframe = m_sceneManager->AddComponent<WireframeMeshComponent>(m_sceneId, m_upperSurfaceBarycentricSubDivisionEntity);
	wireframe->SetMesh(mesh);



	/*Entity entity = m_sceneManager->CreateNewEntity();
	m_sceneManager->AddComponent<StaticMeshComponent>(m_sceneId, entity, mesh);
	std::shared_ptr<TransformComponent> transformComponent2 = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, entity);

	XMMATRIX mat = XMMatrixMultiply(m_sceneSettings.UpperSurfaceBarycentricSubDivisionModel, XMMatrixTranslation(0.0f, 20.0f, 0.0f));

	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, mat);
	transformComponent2->SetScale(scale);
	transformComponent2->SetRotation(rotationQuaternion);
	transformComponent2->SetPosition(transform);*/

	return DsrResult::Success("Register Upper surface Barycentric subdivision success.");
}
