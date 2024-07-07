#pragma once

#include "dsrpch.h"

#include "Data/Pathfinding/AStarStaticMeshPathfinder.h"

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

	std::vector<float> ConstructUpperSurfacePath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);
	std::vector<float> ConstructUpperSurfaceSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);
	std::vector<float> ConstructUpperSurfaceBarycentricSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);
private:
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> m_upperSurfacePathfinder;
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> m_upperSurfaceSubDivisionPathfinder;
	std::shared_ptr<dsr::data::pathfinding::AStarStaticMeshPathfinder> m_upperSurfaceBarycentricSubDivisionPathfinder;

	std::vector<float> ConstructPath(
		const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& finish,
		dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder,
		const DirectX::XMVECTORF32& color
	);
};
