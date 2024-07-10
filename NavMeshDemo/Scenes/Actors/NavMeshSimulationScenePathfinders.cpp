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

std::pair<std::vector<float>, PathSearchStats> NavMeshSimulationScenePathfinders::ConstructUpperSurfacePath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	return ConstructPath(start, finish, *m_upperSurfacePathfinder, DirectX::Colors::Blue);
}

std::pair<std::vector<float>, PathSearchStats> NavMeshSimulationScenePathfinders::ConstructUpperSurfaceSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	return ConstructPath(start, finish, *m_upperSurfaceSubDivisionPathfinder, DirectX::Colors::Green);
}

std::pair<std::vector<float>, PathSearchStats> NavMeshSimulationScenePathfinders::ConstructUpperSurfaceBarycentricSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	return ConstructPath(start, finish, *m_upperSurfaceBarycentricSubDivisionPathfinder, DirectX::Colors::Red);
}

std::pair<std::vector<float>, PathSearchStats> NavMeshSimulationScenePathfinders::ConstructPath(
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

	PathSearchStats stats;
	stats.IndexSearchResultType = result.GetResultType();
	stats.StartIndex = result.GetStartIndex();
	stats.FinishIndex = result.GetFinishIndex();
	stats.NodesTraveled = 0;

	switch (result.GetResultType())
	{
	case VertexIndexSearchResultType::CoTriangular:
		return std::pair(BuildVertexBufferCoTriangular(start, finish, color), stats);
	case VertexIndexSearchResultType::Concurrent:
	{
		stats.NodesTraveled = 1;
		return std::pair(BuildVertexBufferConcurrent(start, finish, pathfinder.GetConnectionVertex(result), color), stats);
	}
	case VertexIndexSearchResultType::PathSearchRequired:
	{
		std::vector<uint32_t> path = pathfinder.Search(result.GetStartIndex(), result.GetFinishIndex());
		stats.NodesTraveled = path.size();

		return std::pair(BuildVertexBuffer(start, finish, path, pathfinder.GetNavMesh().GetVertexBuffer(), color), stats);
	}
	default:
		return std::pair(std::vector<float>(), stats);
	}
}
