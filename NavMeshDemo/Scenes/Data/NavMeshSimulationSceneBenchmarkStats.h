#pragma once

#include "dsrpch.h"

#include "Data/Pathfinding/VertexIndexSearchResult.h"

class NavMeshSimulationSceneBenchmarkStats
{
public:
	dsr::data::pathfinding::VertexIndexSearchResultType GetVertexIndexSearchResultType() const { return m_vertexIndexSearchResultType; }
	void SetVertexIndexSearchResultType(const dsr::data::pathfinding::VertexIndexSearchResultType vertexIndexSearchResultType);

	std::string GetVertexIndexSearchResultTypeText() const { return m_vertexIndexSearchResultTypeText; }

	double GetAvgConnectivity() const { return m_avgConnectivity; }
	void SetAvgConnectivity(const double avgConnectivity) { m_avgConnectivity = avgConnectivity; }

	uint32_t GetNodesTraveled() const { return m_nodesTraveled; }
	void SetNodesTraveled(const uint32_t nodesTraveled) { m_nodesTraveled = nodesTraveled; }

	float GetPathLength() const { return m_pathLength; }
	void SetPathLength(const float pathLength) { m_pathLength = pathLength; }

	float GetTotalPathLength() const { return m_totalPathLength; }
	void SetTotalPathLength(const float totalPathLength) { m_totalPathLength = totalPathLength; }

	size_t GetNavMeshTriangleCount() const { return m_navMeshTriangleCount; }
	void SetNavMeshTriangleCount(const size_t navMeshTriangleCount) { m_navMeshTriangleCount = navMeshTriangleCount; }

	NavMeshSimulationSceneBenchmarkStats();
private:
	dsr::data::pathfinding::VertexIndexSearchResultType m_vertexIndexSearchResultType;
	std::string m_vertexIndexSearchResultTypeText;

	double m_avgConnectivity;
	uint32_t m_nodesTraveled;
	float m_pathLength;
	float m_totalPathLength;
	size_t m_navMeshTriangleCount;
};
