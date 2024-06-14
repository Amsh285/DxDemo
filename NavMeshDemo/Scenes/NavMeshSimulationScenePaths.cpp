#include "NavMeshSimulationScenePaths.h"

#include "Data/Manipulation/StaticMeshExtensions.h"

#include "EngineSubSystems/EntityComponentSystem/Components/LineListComponent.h"

#include "Infrastructure/XMathHelper.h"

NavMeshSimulationScenePaths::NavMeshSimulationScenePaths(
	const uint32_t& sceneId,
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device
) : m_sceneId(sceneId), m_sceneManager(sceneManager), m_device(device)
{
	m_baseMeshPathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfacePathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceSurfaceSubDivisionPathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceBarycentricSubDivisionPathEntity = m_sceneManager->CreateNewEntity();
}

void NavMeshSimulationScenePaths::Setup(
	const NavMeshSimulationSceneSettings& settings,
	std::shared_ptr<dsr::WavefrontModel> upperSurface,
	std::shared_ptr<dsr::WavefrontModel> upperSurfaceSubDivision,
	std::shared_ptr<dsr::WavefrontModel> upperSurfaceBarycentricSubDivision
)
{
	using namespace dsr;
	using namespace dsr::data::manipulation;

	m_upperSurface = FilterDistinct(*upperSurface->Mesh);
	m_upperSurfaceSubDivision = FilterDistinct(*upperSurfaceSubDivision->Mesh);
	m_upperSurfaceBarycentricSubDivision = FilterDistinct(*upperSurfaceBarycentricSubDivision->Mesh);
	m_upperSurface.RefreshHitTestCache();
	m_upperSurfaceSubDivision.RefreshHitTestCache();
	m_upperSurfaceBarycentricSubDivision.RefreshHitTestCache();

	m_upperSurfacePathfinder.SetGraph(m_upperSurface);
	m_upperSurfaceSurfaceSubDivisionPathfinder.SetGraph(m_upperSurfaceSubDivision);
	m_upperSurfaceBarycentricSubDivisionPathfinder.SetGraph(m_upperSurfaceBarycentricSubDivision);

	SetupPathLineEntity(m_baseMeshPathEntity, "BaseMeshPath", settings.BaseMeshModel);
	SetupPathLineEntity(m_upperSurfacePathEntity, "UpperSurfacePath", settings.UpperSurfaceModel);
	SetupPathLineEntity(m_upperSurfaceSurfaceSubDivisionPathEntity, "UpperSurfaceSubDivisionPath", settings.UpperSurfaceSubDivisonModel);
	SetupPathLineEntity(m_upperSurfaceBarycentricSubDivisionPathEntity, "UpperSurfaceBarycentricSubDivisionPath", settings.UpperSurfaceBarycentricSubDivisionModel);
}

dsr::DsrResult NavMeshSimulationScenePaths::SetPaths(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	using namespace dsr;

	using namespace DirectX;

	std::variant<std::vector<float>, dsr_error> constructUpperSurfacePathResult = ConstructPath(
		start,
		finish,
		m_upperSurface,
		m_upperSurfacePathfinder,
		Colors::BlueViolet);

	if (std::holds_alternative<dsr_error>(constructUpperSurfacePathResult))
	{
		const dsr_error& error = std::get<dsr_error>(constructUpperSurfacePathResult);
		std::string errorMessage = "Error constructing Path for Upper surface: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_CONSTRUCTPATH_UPPERSURFACE);
	}

	const std::vector<float>& upperSurfacePath = std::get<std::vector<float>>(constructUpperSurfacePathResult);

	DsrResult setUpperSurfacePathResult = SetPath(m_upperSurfacePathEntity, upperSurfacePath);
	if (setUpperSurfacePathResult.GetResultStatusCode() != RESULT_SUCCESS)
		return setUpperSurfacePathResult;

	DsrResult setUpperSurfacePathOnBaseMeshResult = SetPath(m_baseMeshPathEntity, upperSurfacePath);
	if (setUpperSurfacePathOnBaseMeshResult.GetResultStatusCode() != RESULT_SUCCESS)
		return setUpperSurfacePathOnBaseMeshResult;

	return DsrResult::Success("SetPaths Success.");
}

void NavMeshSimulationScenePaths::SetupPathLineEntity(
	const dsr::ecs::Entity entity,
	const std::string& name,
	const DirectX::XMMATRIX& model
)
{
	using namespace dsr::ecs;

	using namespace DirectX;

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, entity, name);
	m_sceneManager->AddComponent<TagComponent>(m_sceneId, entity, PATHLINE_TAG);

	std::shared_ptr<TransformComponent> transformComponent = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, entity);

	XMVECTOR scale, rotationQuaternion, transform;
	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, model);
	transformComponent->SetScale(scale);
	transformComponent->SetRotation(rotationQuaternion);
	transformComponent->SetPosition(transform);
}

std::variant<std::vector<float>, dsr::dsr_error> NavMeshSimulationScenePaths::ConstructPath(
	const DirectX::XMVECTOR& start,
	const DirectX::XMVECTOR& finish,
	const dsr::data::StaticMesh<dsr::data::Vertex3F>& mesh,
	dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder,
	const DirectX::XMVECTORF32& color
)
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::pathfinding;

	using namespace DirectX;

	std::variant<VertexIndexSearchResult, NotFoundError> searchResult = SearchNearestVertexIndices(start, finish, mesh);

	if (std::holds_alternative<NotFoundError>(searchResult))
		return dsr_error(std::get<NotFoundError>(searchResult).what(), ERROR_SETPATH_TRIANGLESNOTFOUND);

	const VertexIndexSearchResult result = std::get<VertexIndexSearchResult>(searchResult);

	if (result.IsCoTriangular())
	{
		return BuildVertexBufferCoTriangular(start, finish, color);
	}
	else if (result.IsConcurrent())
	{
		const std::vector<Vertex3F>& vertexBuffer = mesh.GetVertexBuffer();
		return BuildVertexBufferConcurrent(
			start,
			finish,
			XMLoadFloat3(&vertexBuffer[result.GetStartIndex()].Position),
			color
		);
	}

	std::vector<uint32_t> path = pathfinder.SearchSequential(result.GetStartIndex(), result.GetFinishIndex());
	return BuildVertexBuffer(start, finish, path, mesh.GetVertexBuffer(), color);
}

dsr::DsrResult NavMeshSimulationScenePaths::SetPath(
	const dsr::ecs::Entity& entity,
	const std::vector<float>& pathBuffer
)
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;

	using namespace dsr::directX;

	using namespace dsr::ecs;

	using namespace DirectX;

	m_sceneManager->RemoveComponent<LineListComponent>(m_sceneId, entity);



	std::variant<Direct3dBuffer, dsr_error> createBufferResult = Direct3dBuffer::CreateVertexBufferf(
		m_device, pathBuffer
	);

	if (std::holds_alternative<dsr_error>(createBufferResult))
	{
		const dsr_error& error = std::get<dsr_error>(createBufferResult);
		return DsrResult(error.what(), ERROR_SETPATH_SETUPD3D11BUFFER);
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	std::shared_ptr<LineListComponent> lineListComponent = m_sceneManager
		->AddComponent<LineListComponent>(m_sceneId, entity);

	lineListComponent->SetStartVertexLocation(0);
	lineListComponent->SetVertexCount(pathBuffer.size() / 7);
	lineListComponent->SetVertexShaderInputLayout(inputLayout);
	lineListComponent->SetVertexBuffer(std::get<Direct3dBuffer>(createBufferResult));

	return DsrResult::Success("Set Path Success on Entity: " + entity);
}
