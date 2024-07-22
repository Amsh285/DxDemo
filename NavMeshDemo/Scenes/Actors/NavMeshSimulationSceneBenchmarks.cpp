#include "NavMeshSimulationSceneBenchmarks.h"

NavMeshSimulationSceneBenchmarks::NavMeshSimulationSceneBenchmarks()
{
	UpperSurfaceBenchmarkHandle = UpperSurfaceBenchmark.CreateHandle();
	UpperSurfaceSubDivisionBenchmarkHandle = UpperSurfaceSubDivisionBenchmark.CreateHandle();
	UpperSurfaceBarycentricSubDivisionBenchmarkHandle = UpperSurfaceBarycentricSubDivisionBenchmark.CreateHandle();
}

void NavMeshSimulationSceneBenchmarks::Update()
{
	UpperSurfaceBenchmark.Update(UpperSurfaceBenchmarkHandle);
	UpperSurfaceSubDivisionBenchmark.Update(UpperSurfaceSubDivisionBenchmarkHandle);
	UpperSurfaceBarycentricSubDivisionBenchmark.Update(UpperSurfaceBarycentricSubDivisionBenchmarkHandle);
}
