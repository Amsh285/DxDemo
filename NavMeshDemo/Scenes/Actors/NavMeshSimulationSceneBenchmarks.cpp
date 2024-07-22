#include "NavMeshSimulationSceneBenchmarks.h"

NavMeshSimulationSceneBenchmarks::NavMeshSimulationSceneBenchmarks()
{
	UpperSurfaceBenchmarkHandle = UpperSurfaceBenchmark.CreateHandle();
}

void NavMeshSimulationSceneBenchmarks::Update()
{
	UpperSurfaceBenchmark.Update(UpperSurfaceBenchmarkHandle);
}
