#include "NavMeshSimulationSceneMediator.h"

void NavMeshSimulationSceneMediator::SetUpperSurfaceSubDivision(const uint32_t count)
{
	using namespace dsr;

	using namespace dsr::data::manipulation;

	m_upperSurfaceSubDivision->SubDivide(count);
	m_pathfinders->SetUpperSurfaceSubDivisionMesh(FilterDistinct(m_upperSurfaceSubDivision->GetSubDividedMesh()));

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

	//Todo: Same here
	SetPaths(m_markers->GetStartPositionLocal(), m_markers->GetFinishPositionLocal());
}

void NavMeshSimulationSceneMediator::SetPaths(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish)
{
	using namespace dsr;

	using namespace dsr::data::pathfinding;

	std::pair<std::vector<float>, VertexIndexSearchResultType> upperSurfaceResult = m_pathfinders->ConstructUpperSurfacePath(start, finish);
	std::pair<std::vector<float>, VertexIndexSearchResultType> upperSurfaceSubDivisionResult = m_pathfinders->ConstructUpperSurfaceSubDivisionPath(start, finish);
	std::pair<std::vector<float>, VertexIndexSearchResultType> upperSurfaceBarycentricSubDivisionResult = m_pathfinders->ConstructUpperSurfaceBarycentricSubDivisionPath(start, finish);

	m_benchmarks->UpperSurfaceStats.CanExecuteBenchmark = upperSurfaceResult.second == VertexIndexSearchResultType::PathSearchRequired;
	m_benchmarks->UpperSurfaceSubDivisionStats.CanExecuteBenchmark = upperSurfaceSubDivisionResult.second == VertexIndexSearchResultType::PathSearchRequired;
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.CanExecuteBenchmark = upperSurfaceBarycentricSubDivisionResult.second == VertexIndexSearchResultType::PathSearchRequired;
	m_benchmarks->UpperSurfaceStats.VertexIndexSearchResultType = upperSurfaceResult.second;
	m_benchmarks->UpperSurfaceSubDivisionStats.VertexIndexSearchResultType = upperSurfaceSubDivisionResult.second;
	m_benchmarks->UpperSurfaceBarycentricSubDivisionStats.VertexIndexSearchResultType = upperSurfaceBarycentricSubDivisionResult.second;

	std::vector<float> baseMeshPath;
	baseMeshPath.insert(baseMeshPath.end(), upperSurfaceResult.first.begin(), upperSurfaceResult.first.end());
	baseMeshPath.insert(baseMeshPath.end(), upperSurfaceSubDivisionResult.first.begin(), upperSurfaceSubDivisionResult.first.end());
	baseMeshPath.insert(baseMeshPath.end(), upperSurfaceBarycentricSubDivisionResult.first.begin(), upperSurfaceBarycentricSubDivisionResult.first.end());

	m_paths->SetBaseMeshPath(baseMeshPath);
	m_paths->SetUpperSurfacePath(upperSurfaceResult.first);
	m_paths->SetUpperSurfaceSubDivisionPath(upperSurfaceSubDivisionResult.first);
	m_paths->SetUpperSurfaceBarycentricSubDivisionPath(upperSurfaceBarycentricSubDivisionResult.first);
}
