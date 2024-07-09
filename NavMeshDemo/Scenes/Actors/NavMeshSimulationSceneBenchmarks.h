#pragma once

#include "dsrpch.h"

struct NavMeshSimulationSceneBenchmarkStats
{
	bool CanExecuteBenchmark = false;
};

class NavMeshSimulationSceneBenchmarks
{
public:
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceSubDivisionStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceBarycentricSubDivisionStats;
private:
};
