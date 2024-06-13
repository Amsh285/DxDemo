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
	std::shared_ptr<dsr::WavefrontModel> baseMesh,
	std::shared_ptr<dsr::WavefrontModel> upperSurface,
	std::shared_ptr<dsr::WavefrontModel> upperSurfaceSubDivision,
	std::shared_ptr<dsr::WavefrontModel> upperSurfaceBarycentricSubDivision
)
{
	using namespace dsr;
	using namespace dsr::data::manipulation;

	m_baseMesh = FilterDistinct(*baseMesh->Mesh);
	m_upperSurface = FilterDistinct(*upperSurface->Mesh);
	m_upperSurfaceSubDivision = FilterDistinct(*upperSurfaceSubDivision->Mesh);
	m_upperSurfaceBarycentricSubDivision = FilterDistinct(*upperSurfaceBarycentricSubDivision->Mesh);
	m_baseMesh.RefreshHitTestCache();
	m_upperSurface.RefreshHitTestCache();
	m_upperSurfaceSubDivision.RefreshHitTestCache();
	m_upperSurfaceBarycentricSubDivision.RefreshHitTestCache();

	m_baseMeshPathfinder.SetGraph(m_baseMesh);
	m_upperSurfacePathfinder.SetGraph(m_upperSurface);
	m_upperSurfaceSurfaceSubDivisionPathfinder.SetGraph(m_upperSurfaceSubDivision);
	m_upperSurfaceBarycentricSubDivisionPathfinder.SetGraph(m_upperSurfaceBarycentricSubDivision);
}

dsr::DsrResult NavMeshSimulationScenePaths::SetPaths(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	using namespace dsr;


	return DsrResult::Success("SetPaths Success.");
}

dsr::DsrResult NavMeshSimulationScenePaths::SetPath(
	const DirectX::XMVECTOR& start,
	const DirectX::XMVECTOR& finish,
	const dsr::ecs::Entity& entity,
	const dsr::data::StaticMesh<dsr::data::Vertex3F>& mesh,
	dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder
)
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::data::manipulation;

	using namespace dsr::directX;

	using namespace dsr::ecs;

	using namespace DirectX;

	

	/*if (!startTriangle.has_value() || !finishTriangle.has_value())
		return DsrResult("Could not find Triangles by intersection.", ERROR_SETPATH_TRIANGLESNOTFOUND);*/

	std::vector<uint32_t> path;

	
	{
		

		
		path = pathfinder.SearchSequential(closestVertices.first, closestVertices.second);
	}

	std::vector<float> vertexData = BuildVertexBuffer(start, finish, path, mesh.GetVertexBuffer(), Colors::Red);

	m_sceneManager->RemoveComponent<LineListComponent>(m_sceneId, entity);




	return DsrResult::Success("Set Path Success on Entity: " + entity);
}
