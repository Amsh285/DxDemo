#include "NavMeshSimulationSceneBenchmarkStats.h"

void NavMeshSimulationSceneBenchmarkStats::SetVertexIndexSearchResultType(const dsr::data::pathfinding::VertexIndexSearchResultType vertexIndexSearchResultType)
{
	using namespace dsr::data::pathfinding;

	m_vertexIndexSearchResultType = vertexIndexSearchResultType;

	switch (vertexIndexSearchResultType)
	{
	case VertexIndexSearchResultType::NoIntersection:
		m_vertexIndexSearchResultTypeText = "No-Intersection";
		break;
	case VertexIndexSearchResultType::CoTriangular:
		m_vertexIndexSearchResultTypeText = "Co-Triangular";
		break;
	case VertexIndexSearchResultType::Concurrent:
		m_vertexIndexSearchResultTypeText = "Concurrent";
		break;
	case VertexIndexSearchResultType::PathSearchRequired:
		m_vertexIndexSearchResultTypeText = "Pathsearch required";
		break;
	default:
		m_vertexIndexSearchResultTypeText = "Unknown";
		break;
	}
}

NavMeshSimulationSceneBenchmarkStats::NavMeshSimulationSceneBenchmarkStats()
	: m_vertexIndexSearchResultType(dsr::data::pathfinding::VertexIndexSearchResultType::Undefined),
	m_vertexIndexSearchResultTypeText("Undefined"), m_averageBranchingFactor(0), m_nodesTraveled(0),
	m_pathLength(0.0f), m_navMeshTriangleCount(0)
{
}
