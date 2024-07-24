#pragma once

#include "dsrpch.h"

#include "Data/Pathfinding/AStarStaticMeshPathfinder.h"

struct PathSearchStats
{
	dsr::data::pathfinding::VertexIndexSearchResultType IndexSearchResultType;
	uint32_t NodesTraveled;
	float PathLength;
	uint32_t StartIndex;
	uint32_t FinishIndex;
};

class NavMeshSimulationScenePathfinders
{
public:
	void SetUpperSurfaceMesh(const dsr::data::StaticMesh<dsr::data::Vertex3F>& navMesh);
	void SetUpperSurfaceSubDivisionMesh(const dsr::data::StaticMesh<dsr::data::Vertex3F>& navMesh);
	void SetUpperSurfaceBarycentricSubDivisionMesh(const dsr::data::StaticMesh<dsr::data::Vertex3F>& navMesh);

	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> GetUpperSurfacePathfinder() const { return m_upperSurfacePathfinder; }
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> GetUpperSurfaceSubDivisionPathfinder() const { return m_upperSurfaceSubDivisionPathfinder; }
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> GetUpperSurfaceBarycentricSubDivisionPathfinder() const { return m_upperSurfaceBarycentricSubDivisionPathfinder; }
	
	NavMeshSimulationScenePathfinders();

	std::pair<std::vector<float>, PathSearchStats> ConstructUpperSurfacePath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);
	std::pair<std::vector<float>, PathSearchStats> ConstructUpperSurfaceSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);
	std::pair<std::vector<float>, PathSearchStats> ConstructUpperSurfaceBarycentricSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);
private:
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> m_upperSurfacePathfinder;
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> m_upperSurfaceSubDivisionPathfinder;
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> m_upperSurfaceBarycentricSubDivisionPathfinder;

	std::pair<std::vector<float>, PathSearchStats> ConstructPath(
		const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& finish,
		dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder,
		const DirectX::XMVECTORF32& color
	);
};
