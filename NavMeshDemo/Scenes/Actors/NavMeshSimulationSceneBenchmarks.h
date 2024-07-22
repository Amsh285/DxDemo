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
	dsr::SyncAccess<NavMeshSimulationSceneBenchmarkResult> UpperSurfaceSubDivisionBenchmark;
	dsr::SyncAccess<NavMeshSimulationSceneBenchmarkResult> UpperSurfaceBarycentricSubDivisionBenchmark;

	dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult> UpperSurfaceBenchmarkHandle;
	dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult> UpperSurfaceSubDivisionBenchmarkHandle;
	dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult> UpperSurfaceBarycentricSubDivisionBenchmarkHandle;

	NavMeshSimulationSceneBenchmarks();

	void Update();
private:
};
