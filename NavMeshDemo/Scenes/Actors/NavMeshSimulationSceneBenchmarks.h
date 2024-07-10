#pragma once

#include "dsrpch.h"

#include "Data/Pathfinding/VertexIndexSearchResult.h"

struct NavMeshSimulationSceneBenchmarkStats
{
	bool CanExecuteBenchmark = false;
	dsr::data::pathfinding::VertexIndexSearchResultType VertexIndexSearchResultType = dsr::data::pathfinding::VertexIndexSearchResultType::NoIntersection;
};

class NavMeshSimulationSceneBenchmarks
{
public:
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceSubDivisionStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceBarycentricSubDivisionStats;
private:
};
