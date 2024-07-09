#include "NavMeshSimulationScenePathfinders.h"

#include "Data/Pathfinding/PathPostProcessor.h"

void NavMeshSimulationScenePathfinders::SetUpperSurfaceMesh(const dsr::data::StaticMesh<dsr::data::Vertex3F>& navMesh)
{
	m_upperSurfacePathfinder->SetGraph(navMesh);
}

void NavMeshSimulationScenePathfinders::SetUpperSurfaceSubDivisionMesh(const dsr::data::StaticMesh<dsr::data::Vertex3F>& navMesh)
{
	m_upperSurfaceSubDivisionPathfinder->SetGraph(navMesh);
}

void NavMeshSimulationScenePathfinders::SetUpperSurfaceBarycentricSubDivisionMesh(const dsr::data::StaticMesh<dsr::data::Vertex3F>& navMesh)
{
	m_upperSurfaceBarycentricSubDivisionPathfinder->SetGraph(navMesh);
}

NavMeshSimulationScenePathfinders::NavMeshSimulationScenePathfinders()
{
	using namespace dsr::data::pathfinding;

	m_upperSurfacePathfinder = std::make_shared<AStarStaticMeshPathfinder>();
	m_upperSurfaceSubDivisionPathfinder = std::make_shared<AStarStaticMeshPathfinder>();
	m_upperSurfaceBarycentricSubDivisionPathfinder = std::make_shared<AStarStaticMeshPathfinder>();
}

std::pair<std::vector<float>, dsr::data::pathfinding::VertexIndexSearchResultType> NavMeshSimulationScenePathfinders::ConstructUpperSurfacePath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	return ConstructPath(start, finish, *m_upperSurfacePathfinder, DirectX::Colors::Blue);
}

std::pair<std::vector<float>, dsr::data::pathfinding::VertexIndexSearchResultType> NavMeshSimulationScenePathfinders::ConstructUpperSurfaceSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	return ConstructPath(start, finish, *m_upperSurfaceSubDivisionPathfinder, DirectX::Colors::Green);
}

std::pair<std::vector<float>, dsr::data::pathfinding::VertexIndexSearchResultType> NavMeshSimulationScenePathfinders::ConstructUpperSurfaceBarycentricSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	return ConstructPath(start, finish, *m_upperSurfaceBarycentricSubDivisionPathfinder, DirectX::Colors::Red);
}

std::pair<std::vector<float>, dsr::data::pathfinding::VertexIndexSearchResultType> NavMeshSimulationScenePathfinders::ConstructPath(
	const DirectX::XMVECTOR& start,
	const DirectX::XMVECTOR& finish,
	dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder,
	const DirectX::XMVECTORF32& color
)
{
	using namespace dsr::data;
	using namespace dsr::data::pathfinding;

	using namespace DirectX;

	VertexIndexSearchResult result = pathfinder.SearchNearestVertexIndices(start, finish);

	switch (result.GetResultType())
	{
	case VertexIndexSearchResultType::CoTriangular:
		return std::pair(BuildVertexBufferCoTriangular(start, finish, color), result.GetResultType());
	case VertexIndexSearchResultType::Concurrent:
		return std::pair(BuildVertexBufferConcurrent(start, finish, pathfinder.GetConnectionVertex(result), color), result.GetResultType());
	case VertexIndexSearchResultType::PathSearchRequired:
	{
		std::vector<uint32_t> path = pathfinder.Search(result.GetStartIndex(), result.GetFinishIndex());
		return std::pair(BuildVertexBuffer(start, finish, path, pathfinder.GetNavMesh().GetVertexBuffer(), color), result.GetResultType());
	}
	default:
		return std::pair(std::vector<float>(), result.GetResultType());
	}
}
