#pragma once

#include "dsrpch.h"

#include "Data/Pathfinding/VertexIndexSearchResult.h"

class NavMeshSimulationSceneBenchmarkStats
{
public:
	dsr::data::pathfinding::VertexIndexSearchResultType GetVertexIndexSearchResultType() const { return m_vertexIndexSearchResultType; }
	void SetVertexIndexSearchResultType(const dsr::data::pathfinding::VertexIndexSearchResultType vertexIndexSearchResultType);

	std::string GetVertexIndexSearchResultTypeText() const { return m_vertexIndexSearchResultTypeText; }

	uint32_t GetAverageBranchingFactor() const { return m_averageBranchingFactor; }
	void SetAverageBranchingFactor(const uint32_t averageBranchingFactor) { m_averageBranchingFactor = averageBranchingFactor; }

	uint32_t GetNodesTraveled() const { return m_nodesTraveled; }
	void SetNodesTraveled(const uint32_t nodesTraveled) { m_nodesTraveled = nodesTraveled; }

	size_t GetNavMeshTriangleCount() const { return m_navMeshTriangleCount; }
	void SetNavMeshTriangleCount(const size_t navMeshTriangleCount) { m_navMeshTriangleCount = navMeshTriangleCount; }

	NavMeshSimulationSceneBenchmarkStats();
private:
	dsr::data::pathfinding::VertexIndexSearchResultType m_vertexIndexSearchResultType;
	std::string m_vertexIndexSearchResultTypeText;

	uint32_t m_averageBranchingFactor;
	uint32_t m_nodesTraveled;
	size_t m_navMeshTriangleCount;
};
