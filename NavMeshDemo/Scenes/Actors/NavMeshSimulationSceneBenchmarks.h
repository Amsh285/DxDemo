#pragma once

#include "dsrpch.h"

#include "Scenes/Data/NavMeshSimulationSceneBenchmarkStats.h"


class NavMeshSimulationSceneBenchmarks
{
public:
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceSubDivisionStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceBarycentricSubDivisionStats;
private:
};
