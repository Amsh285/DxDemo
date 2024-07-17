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

	void SetUpperSurfaceSubDivision(const uint32_t count);
	void SetUpperSurfaceBarycentricSubDivision(const uint32_t count);

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

	NavMeshSimulationSceneBenchmarkResult RunBenchmark(
		dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder,
		const uint32_t iterations
	);
};
