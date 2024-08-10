#pragma once

#include "dsrpch.h"

#include "Scenes/NavMeshSimulationSceneBase.h"

class NavMeshSimulationSceneOverallBenchmarkView final
{
public:
	bool IsBenchmarkRunning() const { return m_benchmarksRunning.load(); }

	void Update(const std::shared_ptr<NavMeshSimulationSceneBase> selectedScene);
private:
	std::atomic<bool> m_benchmarksRunning = false;

	int32_t m_benchmarkIterations = 1000;
};
