#pragma once

#include "dsrpch.h"

#include "Data/Pathfinding/AStarStaticMeshPathfinder.h"
#include "Data/Pathfinding/PathPreProcessor.h"

#include "Data/Structures/StaticMeshTriangle.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

constexpr auto ERROR_SETPATH_TRIANGLESNOTFOUND = 700;

class NavMeshSimulationScenePaths
{
public:
	NavMeshSimulationScenePaths(
		const uint32_t& sceneId,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device
	);

	void Setup(
		std::shared_ptr<dsr::WavefrontModel> baseMesh,
		std::shared_ptr<dsr::WavefrontModel> upperSurface,
		std::shared_ptr<dsr::WavefrontModel> upperSurfaceSubDivision,
		std::shared_ptr<dsr::WavefrontModel> upperSurfaceBarycentricSubDivision
	);

	dsr::DsrResult SetPaths(
		const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& finish
	);
private:
	uint32_t m_sceneId;

	dsr::ecs::Entity m_baseMeshPathEntity;
	dsr::ecs::Entity m_upperSurfacePathEntity;
	dsr::ecs::Entity m_upperSurfaceSurfaceSubDivisionPathEntity;
	dsr::ecs::Entity m_upperSurfaceBarycentricSubDivisionPathEntity;

	dsr::data::StaticMesh<dsr::data::Vertex3F> m_baseMesh;
	dsr::data::StaticMesh<dsr::data::Vertex3F> m_upperSurface;
	dsr::data::StaticMesh<dsr::data::Vertex3F> m_upperSurfaceSubDivision;
	dsr::data::StaticMesh<dsr::data::Vertex3F> m_upperSurfaceBarycentricSubDivision;

	dsr::data::pathfinding::AStarStaticMeshPathfinder m_baseMeshPathfinder;
	dsr::data::pathfinding::AStarStaticMeshPathfinder m_upperSurfacePathfinder;
	dsr::data::pathfinding::AStarStaticMeshPathfinder m_upperSurfaceSurfaceSubDivisionPathfinder;
	dsr::data::pathfinding::AStarStaticMeshPathfinder m_upperSurfaceBarycentricSubDivisionPathfinder;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;

	dsr::DsrResult SetPath(
		const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& finish,
		const dsr::ecs::Entity& entity,
		const dsr::data::StaticMesh<dsr::data::Vertex3F>& mesh,
		dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder
	);
};
