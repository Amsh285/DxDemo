#include "NavMeshSimulationSceneMediator.h"

void NavMeshSimulationSceneMediator::Setup()
{
	m_benchmarks->UpperSurfaceStats.SetNavMeshTriangleCount(m_pathfinders->GetUpperSurfacePathfinder()->GetNavMesh().GetIndexBuffer().size() / 3);
	m_benchmarks->UpperSurfaceSubDivisionStats.SetNavMeshTriangleCount(m_pathfinders->GetUpperSurfaceSubDivisionPathfinder()->GetNavMesh().GetIndexBuffer().size() / 3);
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.SetNavMeshTriangleCount(m_pathfinders->GetUpperSurfaceBarycentricSubDivisionPathfinder()->GetNavMesh().GetIndexBuffer().size() / 3);

	m_benchmarks->UpperSurfaceStats.SetAverageBranchingFactor(m_pathfinders->GetUpperSurfacePathfinder()->GetAverageBranchingFactor());
	m_benchmarks->UpperSurfaceSubDivisionStats.SetAverageBranchingFactor(m_pathfinders->GetUpperSurfaceSubDivisionPathfinder()->GetAverageBranchingFactor());
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.SetAverageBranchingFactor(m_pathfinders->GetUpperSurfaceBarycentricSubDivisionPathfinder()->GetAverageBranchingFactor());

	SetPaths(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal());
}

void NavMeshSimulationSceneMediator::RunUpperSurfaceBenchmark(const uint32_t iterations)
{
	using namespace dsr::data::pathfinding;

	NavMeshSimulationSceneBenchmarkResult result = RunBenchmark<heuristics::EuclideanDistance>(*m_pathfinders->GetUpperSurfacePathfinder(), iterations);
	m_benchmarks->UpperSurfaceBenchmark.SetData(std::move(result));
}

void NavMeshSimulationSceneMediator::RunUpperSurfaceSubDivisionBenchmark(const uint32_t iterations)
{
	using namespace dsr::data::pathfinding;

	NavMeshSimulationSceneBenchmarkResult result = RunBenchmark<heuristics::EuclideanDistance>(*m_pathfinders->GetUpperSurfaceSubDivisionPathfinder(), iterations);
	m_benchmarks->UpperSurfaceSubDivisionBenchmark.SetData(std::move(result));
}

void NavMeshSimulationSceneMediator::RunUpperSurfaceBarycentricSubDivisionBenchmark(const uint32_t iterations)
{
	using namespace dsr::data::pathfinding;

	NavMeshSimulationSceneBenchmarkResult result = RunBenchmark<heuristics::EuclideanDistance>(*m_pathfinders->GetUpperSurfaceBarycentricSubDivisionPathfinder(), iterations);
	m_benchmarks->UpperSurfaceBarycentricSubDivisionBenchmark.SetData(std::move(result));
}

void NavMeshSimulationSceneMediator::RunUpperSurfaceDijkstraBenchmark(const uint32_t iterations)
{
	using namespace dsr::data::pathfinding;

	NavMeshSimulationSceneBenchmarkResult result = RunBenchmark<heuristics::Dijsktra>(*m_pathfinders->GetUpperSurfacePathfinder(), iterations);
	m_benchmarks->UpperSurfaceDijkstraBenchmark.SetData(std::move(result));
}

void NavMeshSimulationSceneMediator::RunUpperSurfaceSubDivisionDijkstraBenchmark(const uint32_t iterations)
{
	using namespace dsr::data::pathfinding;

	NavMeshSimulationSceneBenchmarkResult result = RunBenchmark<heuristics::Dijsktra>(*m_pathfinders->GetUpperSurfaceSubDivisionPathfinder(), iterations);
	m_benchmarks->UpperSurfaceSubDivisionDijkstraBenchmark.SetData(std::move(result));
}

void NavMeshSimulationSceneMediator::RunUpperSurfaceBarycentricSubDivisionDijkstraBenchmark(const uint32_t iterations)
{
	using namespace dsr::data::pathfinding;

	NavMeshSimulationSceneBenchmarkResult result = RunBenchmark<heuristics::Dijsktra>(*m_pathfinders->GetUpperSurfaceBarycentricSubDivisionPathfinder(), iterations);
	m_benchmarks->UpperSurfaceBarycentricSubDivisionDijkstraBenchmark.SetData(std::move(result));
}

void NavMeshSimulationSceneMediator::SetUpperSurfaceSubDivision(const uint32_t count)
{
	using namespace dsr;

	using namespace dsr::data::manipulation;

	m_upperSurfaceSubDivision->SubDivide(count);
	m_pathfinders->SetUpperSurfaceSubDivisionMesh(FilterDistinct(m_upperSurfaceSubDivision->GetSubDividedMesh()));

	m_benchmarks->UpperSurfaceSubDivisionStats.SetNavMeshTriangleCount(m_pathfinders->GetUpperSurfaceSubDivisionPathfinder()->GetNavMesh().GetIndexBuffer().size() / 3);
	m_benchmarks->UpperSurfaceSubDivisionStats.SetAverageBranchingFactor(m_pathfinders->GetUpperSurfaceSubDivisionPathfinder()->GetAverageBranchingFactor());

	//Todo: Only update necessary paths
	//m_paths->SetUpperSurfaceSubDivisionPath(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal());
	SetPaths(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal());
}

void NavMeshSimulationSceneMediator::SetUpperSurfaceBarycentricSubDivision(const uint32_t count)
{
	using namespace dsr;

	using namespace dsr::data::manipulation;

	m_upperSurfaceBarycentricSubDivision->SubDivideBarycentric(count);
	m_pathfinders->SetUpperSurfaceBarycentricSubDivisionMesh(FilterDistinct(m_upperSurfaceBarycentricSubDivision->GetSubDividedMesh()));

	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.SetNavMeshTriangleCount(m_pathfinders->GetUpperSurfaceBarycentricSubDivisionPathfinder()->GetNavMesh().GetIndexBuffer().size() / 3);
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.SetAverageBranchingFactor(m_pathfinders->GetUpperSurfaceBarycentricSubDivisionPathfinder()->GetAverageBranchingFactor());

	//Todo: Same here
	SetPaths(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal());
}

void NavMeshSimulationSceneMediator::SetPaths(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	using namespace dsr;

	using namespace dsr::data::pathfinding;

	std::pair<std::vector<float>, PathSearchStats> upperSurfaceResult = m_pathfinders->ConstructUpperSurfacePath(start, finish);
	std::pair<std::vector<float>, PathSearchStats> upperSurfaceSubDivisionResult = m_pathfinders->ConstructUpperSurfaceSubDivisionPath(start, finish);
	std::pair<std::vector<float>, PathSearchStats> upperSurfaceBarycentricSubDivisionResult = m_pathfinders->ConstructUpperSurfaceBarycentricSubDivisionPath(start, finish);

	m_benchmarks->UpperSurfaceStats.SetVertexIndexSearchResultType(upperSurfaceResult.second.IndexSearchResultType);
	m_benchmarks->UpperSurfaceSubDivisionStats.SetVertexIndexSearchResultType(upperSurfaceSubDivisionResult.second.IndexSearchResultType);
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.SetVertexIndexSearchResultType(upperSurfaceBarycentricSubDivisionResult.second.IndexSearchResultType);

	m_benchmarks->UpperSurfaceStats.SetNodesTraveled(upperSurfaceResult.second.NodesTraveled);
	m_benchmarks->UpperSurfaceSubDivisionStats.SetNodesTraveled(upperSurfaceSubDivisionResult.second.NodesTraveled);
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.SetNodesTraveled(upperSurfaceBarycentricSubDivisionResult.second.NodesTraveled);

	m_benchmarks->UpperSurfaceStats.SetPathLength(upperSurfaceResult.second.PathLength);
	m_benchmarks->UpperSurfaceSubDivisionStats.SetPathLength(upperSurfaceSubDivisionResult.second.PathLength);
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.SetPathLength(upperSurfaceBarycentricSubDivisionResult.second.PathLength);

	std::vector<float> baseMeshPath;
	baseMeshPath.insert(baseMeshPath.end(), upperSurfaceResult.first.begin(), upperSurfaceResult.first.end());
	baseMeshPath.insert(baseMeshPath.end(), upperSurfaceSubDivisionResult.first.begin(), upperSurfaceSubDivisionResult.first.end());
	baseMeshPath.insert(baseMeshPath.end(), upperSurfaceBarycentricSubDivisionResult.first.begin(), upperSurfaceBarycentricSubDivisionResult.first.end());

	m_paths->SetBaseMeshPath(baseMeshPath);
	m_paths->SetUpperSurfacePath(upperSurfaceResult.first);
	m_paths->SetUpperSurfaceSubDivisionPath(upperSurfaceSubDivisionResult.first);
	m_paths->SetUpperSurfaceBarycentricSubDivisionPath(upperSurfaceBarycentricSubDivisionResult.first);
}

void NavMeshSimulationSceneMediator::ResetBenchmarks()
{
	using namespace std::chrono;

	NavMeshSimulationSceneBenchmarkResult upperSurfaceBenchmarkDefaults;
	upperSurfaceBenchmarkDefaults.TotalTime = duration<double, std::nano>::zero();
	upperSurfaceBenchmarkDefaults.AverageIterationTime = duration<double, std::nano>::zero();
	upperSurfaceBenchmarkDefaults.StandardDeviationTime = duration<double, std::nano>::zero();
	m_benchmarks->UpperSurfaceBenchmark.SetData(std::move(upperSurfaceBenchmarkDefaults));

	NavMeshSimulationSceneBenchmarkResult upperSurfaceSubDivisionBenchmarkDefaults;
	upperSurfaceSubDivisionBenchmarkDefaults.TotalTime = duration<double, std::nano>::zero();
	upperSurfaceSubDivisionBenchmarkDefaults.AverageIterationTime = duration<double, std::nano>::zero();
	upperSurfaceSubDivisionBenchmarkDefaults.StandardDeviationTime = duration<double, std::nano>::zero();
	m_benchmarks->UpperSurfaceSubDivisionBenchmark.SetData(std::move(upperSurfaceSubDivisionBenchmarkDefaults));

	NavMeshSimulationSceneBenchmarkResult upperSurfaceBarycentricSubDivisionBenchmarkDefaults;
	upperSurfaceBarycentricSubDivisionBenchmarkDefaults.TotalTime = duration<double, std::nano>::zero();
	upperSurfaceBarycentricSubDivisionBenchmarkDefaults.AverageIterationTime = duration<double, std::nano>::zero();
	upperSurfaceBarycentricSubDivisionBenchmarkDefaults.StandardDeviationTime = duration<double, std::nano>::zero();
	m_benchmarks->UpperSurfaceBarycentricSubDivisionBenchmark.SetData(std::move(upperSurfaceBarycentricSubDivisionBenchmarkDefaults));
}
