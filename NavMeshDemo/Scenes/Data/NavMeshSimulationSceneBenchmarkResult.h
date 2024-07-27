#pragma once

#include "dsrpch.h"

struct NavMeshSimulationSceneBenchmarkResult
{
	std::chrono::duration<double, std::nano> TotalTime;
	std::chrono::duration<double, std::nano> AverageIterationTime;
	std::chrono::duration<double, std::nano> StandardDeviationTime;
	std::vector<std::chrono::duration<double, std::nano>> IterationTimes;

	uint32_t NodesTraveled;
	float PathLength;

	NavMeshSimulationSceneBenchmarkResult();
};
