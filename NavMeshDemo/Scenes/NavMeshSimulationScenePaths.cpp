#include "NavMeshSimulationScenePaths.h"

#include "Data/Manipulation/StaticMeshExtensions.h"

#include "EngineSubSystems/EntityComponentSystem/Components/LineListComponent.h"

#include "Infrastructure/XMathHelper.h"

void NavMeshSimulationScenePaths::SetUpperSurfaceSubDivision(const dsr::data::StaticMesh<dsr::data::Vertex3F>& upperSurfaceSubDivision)
{
	m_upperSurfaceSubDivision = upperSurfaceSubDivision;
	m_upperSurfaceSubDivisionPathfinder.SetGraph(m_upperSurfaceSubDivision);
}

dsr::DsrResult NavMeshSimulationScenePaths::SetUpperSurfaceSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	using namespace dsr;

	using namespace DirectX;

	std::variant<std::vector<float>, dsr_error> constructUpperSurfacePathSubDivisionResult = ConstructPath(
		start,
		finish,
		m_upperSurfaceSubDivision,
		m_upperSurfaceSubDivisionPathfinder,
		Colors::Green
	);

	if (std::holds_alternative<dsr_error>(constructUpperSurfacePathSubDivisionResult))
	{
		const dsr_error& error = std::get<dsr_error>(constructUpperSurfacePathSubDivisionResult);
		std::string errorMessage = "Error constructing Path for Upper surface SubDivision: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_CONSTRUCTPATH_UPPERSURFACE);
	}

	const std::vector<float>& upperSurfaceSubDivisionPath = std::get<std::vector<float>>(constructUpperSurfacePathSubDivisionResult);

	DsrResult setUpperSurfaceSubDivisionPathResult = SetPath(m_upperSurfaceSubDivisionPathEntity, upperSurfaceSubDivisionPath);
	if (setUpperSurfaceSubDivisionPathResult.GetResultStatusCode() != RESULT_SUCCESS)
		return setUpperSurfaceSubDivisionPathResult;

	return DsrResult::Success("SetUpperSurfaceSubDivisionPath Success.");
}

NavMeshSimulationScenePaths::NavMeshSimulationScenePaths(
	const uint32_t& sceneId,
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device
) : m_sceneId(sceneId), m_sceneManager(sceneManager), m_device(device)
{
	m_baseMeshPathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfacePathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceSubDivisionPathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceBarycentricSubDivisionPathEntity = m_sceneManager->CreateNewEntity();
}

void NavMeshSimulationScenePaths::Setup(
	const NavMeshSimulationSceneSettings& settings,
	std::shared_ptr<dsr::WavefrontModel> upperSurface,
	const dsr::data::StaticMesh<dsr::data::Vertex3F>& upperSurfaceSubDivision,
	std::shared_ptr<dsr::WavefrontModel> upperSurfaceBarycentricSubDivision
)
{
	using namespace dsr;
	using namespace dsr::data::manipulation;

	using namespace DirectX;

	m_upperSurface = FilterDistinct(*upperSurface->Mesh);
	m_upperSurfaceSubDivision = upperSurfaceSubDivision;
	m_upperSurfaceBarycentricSubDivision = FilterDistinct(*upperSurfaceBarycentricSubDivision->Mesh);

	m_upperSurfacePathfinder.SetGraph(m_upperSurface);
	m_upperSurfaceSubDivisionPathfinder.SetGraph(m_upperSurfaceSubDivision);
	m_upperSurfaceBarycentricSubDivisionPathfinder.SetGraph(m_upperSurfaceBarycentricSubDivision);

	XMMATRIX translationOffset = XMMatrixTranslation(0.0, 0.5f, 0.0f);

	SetupPathLineEntity(m_baseMeshPathEntity, "BaseMeshPath", XMMatrixMultiply(settings.BaseMeshModel, translationOffset));
	SetupPathLineEntity(m_upperSurfacePathEntity, "UpperSurfacePath", XMMatrixMultiply(settings.UpperSurfaceModel, translationOffset));
	SetupPathLineEntity(m_upperSurfaceSubDivisionPathEntity, "UpperSurfaceSubDivisionPath", XMMatrixMultiply(settings.UpperSurfaceSubDivisonModel, translationOffset));
	SetupPathLineEntity(m_upperSurfaceBarycentricSubDivisionPathEntity, "UpperSurfaceBarycentricSubDivisionPath", XMMatrixMultiply(settings.UpperSurfaceBarycentricSubDivisionModel, translationOffset));
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
		Colors::Blue);

	std::variant<std::vector<float>, dsr_error> constructUpperSurfacePathSubDivisionResult = ConstructPath(
		start,
		finish,
		m_upperSurfaceSubDivision,
		m_upperSurfaceSubDivisionPathfinder,
		Colors::Green
	);

	std::variant<std::vector<float>, dsr_error> constructUpperSurfaceBarycentricSubDivisionResult = ConstructPath(
		start,
		finish,
		m_upperSurfaceBarycentricSubDivision,
		m_upperSurfaceBarycentricSubDivisionPathfinder,
		Colors::Red
	);

	if (std::holds_alternative<dsr_error>(constructUpperSurfacePathResult))
	{
		const dsr_error& error = std::get<dsr_error>(constructUpperSurfacePathResult);
		std::string errorMessage = "Error constructing Path for Upper surface: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_CONSTRUCTPATH_UPPERSURFACE);
	}

	if (std::holds_alternative<dsr_error>(constructUpperSurfacePathSubDivisionResult))
	{
		const dsr_error& error = std::get<dsr_error>(constructUpperSurfacePathSubDivisionResult);
		std::string errorMessage = "Error constructing Path for Upper surface SubDivision: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_CONSTRUCTPATH_UPPERSURFACE);
	}

	if (std::holds_alternative<dsr_error>(constructUpperSurfaceBarycentricSubDivisionResult))
	{
		const dsr_error& error = std::get<dsr_error>(constructUpperSurfaceBarycentricSubDivisionResult);
		std::string errorMessage = "Error constructing Path for Upper surface barycentric SubDivision: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, ERROR_CONSTRUCTPATH_UPPERSURFACE);
	}

	const std::vector<float>& upperSurfacePath = std::get<std::vector<float>>(constructUpperSurfacePathResult);
	const std::vector<float>& upperSurfaceSubDivisionPath = std::get<std::vector<float>>(constructUpperSurfacePathSubDivisionResult);
	const std::vector<float>& upperSurfaceBarycentricSubDivisionPath = std::get<std::vector<float>>(constructUpperSurfaceBarycentricSubDivisionResult);

	std::vector<float> surfacePath;
	surfacePath.insert(surfacePath.end(), upperSurfacePath.begin(), upperSurfacePath.end());
	surfacePath.insert(surfacePath.end(), upperSurfaceSubDivisionPath.begin(), upperSurfaceSubDivisionPath.end());
	surfacePath.insert(surfacePath.end(), upperSurfaceBarycentricSubDivisionPath.begin(), upperSurfaceBarycentricSubDivisionPath.end());

	DsrResult setUpperSurfacePathResult = SetPath(m_upperSurfacePathEntity, upperSurfacePath);
	if (setUpperSurfacePathResult.GetResultStatusCode() != RESULT_SUCCESS)
		return setUpperSurfacePathResult;

	DsrResult setUpperSurfaceSubDivisionPathResult = SetPath(m_upperSurfaceSubDivisionPathEntity, upperSurfaceSubDivisionPath);
	if (setUpperSurfaceSubDivisionPathResult.GetResultStatusCode() != RESULT_SUCCESS)
		return setUpperSurfaceSubDivisionPathResult;

	DsrResult setUpperSuraceBarycentricSubDivisionPathResult = SetPath(m_upperSurfaceBarycentricSubDivisionPathEntity, upperSurfaceBarycentricSubDivisionPath);
	if (setUpperSuraceBarycentricSubDivisionPathResult.GetResultStatusCode() != RESULT_SUCCESS)
		return setUpperSuraceBarycentricSubDivisionPathResult;

	DsrResult setUpperSurfacePathOnBaseMeshResult = SetPath(m_baseMeshPathEntity, surfacePath);
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

	if(pathBuffer.empty())
		return DsrResult::Success("No Path to set on Entity: " + entity);

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
