#pragma once

#include "dsrpch.h"

#include "Scenes/NavMeshSimulationSceneBase.h"
#include "Scenes/Data/NavMeshSimulationSceneBenchmarkStats.h"
#include "Scenes/Data/NavMeshSimulationSceneBenchmarkResult.h"
#include "Scenes/Data/TimeUnit.h"

#include "Infrastructure/SyncAccess.h"

enum class BenchmarkViewType
{
	UpperSurface,
	UpperSurfaceSubDivision,
	UpperSurfaceBarycentricSubDivision
};

class NavMeshSimulationSceneBenchmarkView
{
public:
	bool IsBenchmarkRunning() const { return m_isEuclideanBenchmarkRunning.load() || m_isDijkstraBenchmarkRunning.load(); }

	NavMeshSimulationSceneBenchmarkView(const std::string& controlId, const BenchmarkViewType viewType);

	void Update(
		std::shared_ptr<NavMeshSimulationSceneBase> selectedScene,
		const TimeUnit selectedTimeUnit,
		const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& euclideanBenchmarkHandle,
		const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& dijkstraBenchmarkHandle
	);
private:
	std::atomic<bool> m_isEuclideanBenchmarkRunning = false;
	std::atomic<bool> m_isDijkstraBenchmarkRunning = false;

	int32_t m_euclideanBenchmarkIterations = 1000;
	int32_t m_dijkstraBenchmarkIterations = 1000;

	std::string m_controlId;
	std::string m_benchmarkEnabledLabel;
	std::string m_benchmarkEuclideanIterationTimesLabel;
	std::string m_benchmarkDijkstraIterationTimesLabel;
	std::string m_benchmarkEuclideanRunButtonLabel;
	std::string m_benchmarkDijkstraRunButtonLabel;

	BenchmarkViewType m_viewType;

	NavMeshSimulationSceneBenchmarkStats GetStats(std::shared_ptr<NavMeshSimulationSceneBase> selectedScene) const;
	void RunEuclideanBenchmark(std::shared_ptr<NavMeshSimulationSceneBase> selectedScene);
	void RunDijkstraBenchmark(std::shared_ptr<NavMeshSimulationSceneBase> selectedScene);

	void DisplayBenchmarkResult(const NavMeshSimulationSceneBenchmarkResult& benchmarkResult, const TimeUnit unit);
};
