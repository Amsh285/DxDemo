#pragma once

#include "dsrpch.h"

#include "Data/Structures/StaticMesh.h"

#include "Scenes/Actors/NavMeshSimulationSceneBenchmarks.h"
#include "Scenes/Actors/NavMeshSimulationSceneMarkers.h"
#include "Scenes/Actors/NavMeshSimulationSceneMeshSubDivision.h"
#include "Scenes/Actors/NavMeshSimulationScenePaths.h"
#include "Scenes/Actors/NavMeshSimulationScenePathfinders.h"

#include "Scenes/Data/NavMeshSimulationSceneBenchmarkResult.h"

class NavMeshSimulationSceneMediator
{
public:
	std::shared_ptr<NavMeshSimulationSceneMarkers> GetMarkers() const { return m_markers; }
	void SetMarkers(const std::shared_ptr<NavMeshSimulationSceneMarkers>& markers) { m_markers = markers; }

	std::shared_ptr<NavMeshSimulationScenePaths> GetPaths() const { return m_paths; }
	void SetPaths(const std::shared_ptr<NavMeshSimulationScenePaths>& paths) { m_paths = paths; }

	std::shared_ptr<NavMeshSimulationScenePathfinders> GetPathfinders() const { return m_pathfinders; }
	void SetPathfinders(const std::shared_ptr<NavMeshSimulationScenePathfinders>& pathfinders) { m_pathfinders = pathfinders; }

	const dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN>& GetUpperSurface() const { return m_upperSurface; }
	void SetUpperSurface(const dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN>& upperSurface) { m_upperSurface = upperSurface; }

	std::shared_ptr<NavMeshSimulationSceneMeshSubDivision> GetUpperSurfaceSubDivision() const { return m_upperSurfaceSubDivision; }
	void SetUpperSurfaceSubDivision(const std::shared_ptr<NavMeshSimulationSceneMeshSubDivision>& upperSurfaceSubDivision) { m_upperSurfaceSubDivision = upperSurfaceSubDivision; }

	std::shared_ptr<NavMeshSimulationSceneMeshSubDivision> GetUpperSurfaceBarycentricSubDivision() const { return m_upperSurfaceBarycentricSubDivision; }
	void SetUpperSurfaceBarycentricSubDivision(const std::shared_ptr<NavMeshSimulationSceneMeshSubDivision>& upperSurfaceBarycentricSubDivision) { m_upperSurfaceBarycentricSubDivision = upperSurfaceBarycentricSubDivision; }

	std::shared_ptr<NavMeshSimulationSceneBenchmarks> GetBenchmarks() const { return m_benchmarks; }
	void SetBenchmarks(const std::shared_ptr<NavMeshSimulationSceneBenchmarks>& benchmarks) { m_benchmarks = benchmarks; }

	NavMeshSimulationSceneMediator() = default;

	void Setup();

	void RunUpperSurfaceBenchmark(const uint32_t iterations);
	void RunUpperSurfaceSubDivisionBenchmark(const uint32_t iterations);
	void RunUpperSurfaceBarycentricSubDivisionBenchmark(const uint32_t iterations);

	void RunUpperSurfaceDijkstraBenchmark(const uint32_t iterations);
	void RunUpperSurfaceSubDivisionDijkstraBenchmark(const uint32_t iterations);
	void RunUpperSurfaceBarycentricSubDivisionDijkstraBenchmark(const uint32_t iterations);

	void RunAllBenchmarks(const uint32_t iterations);
	void RunALlBenchmarksParallel(const uint32_t iterations);

	void SetUpperSurfaceSubDivision(const uint32_t count);
	void SetUpperSurfaceBarycentricSubDivision(const uint32_t count);

	void SaveBenchmarkResults(const std::string& sceneName);

	void SetPaths(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);
	void ResetBenchmarks();
private:
	std::shared_ptr<NavMeshSimulationSceneMarkers> m_markers;
	std::shared_ptr<NavMeshSimulationScenePaths> m_paths;
	std::shared_ptr<NavMeshSimulationScenePathfinders> m_pathfinders;
	std::shared_ptr<NavMeshSimulationSceneBenchmarks> m_benchmarks;

	dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN> m_upperSurface;
	std::shared_ptr<NavMeshSimulationSceneMeshSubDivision> m_upperSurfaceSubDivision;
	std::shared_ptr<NavMeshSimulationSceneMeshSubDivision> m_upperSurfaceBarycentricSubDivision;

	template<class THeuristic>
	NavMeshSimulationSceneBenchmarkResult RunBenchmark(
		dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder,
		const uint32_t iterations
	);

	void SaveBenchmark(
		const NavMeshSimulationSceneBenchmarkStats& stats,
		const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& euclideanResultHandle,
		const dsr::SyncHandle<NavMeshSimulationSceneBenchmarkResult>& dijkstraResultHandle,
		const std::string& baseDirectory,
		const std::string benchmarkName
	);

	void WriteLabelStatFile(const std::string& baseDirectory);

	void WriteStatFile(
		const std::string& baseDirectory,
		const std::string& benchmarkName,
		const NavMeshSimulationSceneBenchmarkStats& stats
	);

	void WriteResultLabelFile(const std::string& baseDirectory);

	void WriteResultFile(
		const std::string& baseDirectory,
		const std::string& benchmarkName,
		const std::string& heuristicName,
		const NavMeshSimulationSceneBenchmarkResult& result
	);

	void WriteIterationTimesFile(
		const std::string& baseDirectory,
		const std::string& benchmarkName,
		const std::string& heuristicName,
		const std::vector<std::chrono::duration<double, std::nano>>& iterationTimes
	);
};

template<class THeuristic>
inline NavMeshSimulationSceneBenchmarkResult NavMeshSimulationSceneMediator::RunBenchmark(dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder, const uint32_t iterations)
{
	using namespace dsr::data::pathfinding;

	using namespace std::chrono;

	if(iterations == 0)
		return NavMeshSimulationSceneBenchmarkResult();

	VertexIndexSearchResult indexSearchResult = pathfinder.SearchNearestVertexIndices(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal());
	assert(indexSearchResult.GetResultType() == VertexIndexSearchResultType::PathSearchRequired);

	duration<double, std::nano> sum = duration<double, std::nano>::zero();
	std::vector<duration<double, std::nano>> iterationTimes;

	uint32_t startIndex = indexSearchResult.GetStartIndex();
	uint32_t finishIndex = indexSearchResult.GetFinishIndex();

	time_point<high_resolution_clock> start = high_resolution_clock::now();
	std::vector<uint32_t> path = pathfinder.Search<THeuristic>(startIndex, finishIndex);
	duration<double, std::nano> elapsed = high_resolution_clock::now() - start;
	sum += elapsed;
	iterationTimes.push_back(elapsed);

	for (size_t i = 0; i < iterations - 1; i++)
	{
		time_point<high_resolution_clock> start = high_resolution_clock::now();
		pathfinder.Search<THeuristic>(startIndex, finishIndex);
		duration<double, std::nano> elapsed = high_resolution_clock::now() - start;
		sum += elapsed;
		iterationTimes.push_back(elapsed);
	}

	NavMeshSimulationSceneBenchmarkResult result;
	result.IterationTimes = iterationTimes;
	result.TotalTime = sum;
	result.AverageIterationTime = sum / iterations;

	double sumSquared = 0;

	for (size_t i = 0; i < iterationTimes.size(); i++)
	{
		double time = iterationTimes[i].count() - result.AverageIterationTime.count();
		sumSquared += time * time;
	}

	result.StandardDeviationTime = duration<double, std::nano>(sqrt(sumSquared / iterations));
	result.NodesTraveled = path.size();
	result.PathLength = pathfinder.GetLegnth(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal(), path);

	return result;
}
