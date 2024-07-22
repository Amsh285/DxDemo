#pragma once

#include "dsrpch.h"

#include "Infrastructure/SyncAccess.h"

#include "Scenes/Data/NavMeshSimulationSceneBenchmarkResult.h"
#include "Scenes/Data/NavMeshSimulationSceneBenchmarkStats.h"

class NavMeshSimulationSceneBenchmarks
{
public:
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceSubDivisionStats;
	NavMeshSimulationSceneBenchmarkStats UpperSurfaceBarycentricSubDivisionStats;

	dsr::SyncAccess<NavMeshSimulationSceneBenchmarkResult> UpperSurfaceBenchmark;
	NavMeshSimulationSceneBenchmarkResult UpperSurfaceSubDivisionBenchmark;
	NavMeshSimulationSceneBenchmarkResult UpperSurfaceBarycentricSubDivisionBenchmark;

	dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult> UpperSurfaceBenchmarkHandle;

	NavMeshSimulationSceneBenchmarks();

	void Update();
private:
};
