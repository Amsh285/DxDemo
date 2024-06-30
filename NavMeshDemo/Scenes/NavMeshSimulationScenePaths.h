#pragma once

#include "dsrpch.h"

#include "Data/NavMeshSimulationSceneSettings.h"

#include "Data/Pathfinding/AStarStaticMeshPathfinder.h"
#include "Data/Pathfinding/PathPreProcessor.h"
#include "Data/Pathfinding/PathPostProcessor.h"

#include "Data/Structures/StaticMeshTriangle.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

constexpr const char* PATHLINE_TAG = "Pathlinemarker";

constexpr auto ERROR_SETPATH_TRIANGLESNOTFOUND = 700;

constexpr auto ERROR_CONSTRUCTPATH_UPPERSURFACE = 800;

constexpr auto ERROR_SETPATH_SETUPD3D11BUFFER = 900;

class NavMeshSimulationScenePaths
{
public:

	void SetUpperSurfaceSubDivision(const dsr::data::StaticMesh<dsr::data::Vertex3F>& upperSurfaceSubDivision);
	dsr::DsrResult SetUpperSurfaceSubDivisionPath(const DirectX::XMVECTOR& start, const DirectX::XMVECTOR& finish);

	NavMeshSimulationScenePaths(
		const uint32_t& sceneId,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device
	);

	void Setup(
		const NavMeshSimulationSceneSettings& settings,
		std::shared_ptr<dsr::WavefrontModel> upperSurface,
		const dsr::data::StaticMesh<dsr::data::Vertex3F>& upperSurfaceSubDivision,
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
	dsr::ecs::Entity m_upperSurfaceSubDivisionPathEntity;
	dsr::ecs::Entity m_upperSurfaceBarycentricSubDivisionPathEntity;

	dsr::data::StaticMesh<dsr::data::Vertex3F> m_upperSurface;
	dsr::data::StaticMesh<dsr::data::Vertex3F> m_upperSurfaceSubDivision;
	dsr::data::StaticMesh<dsr::data::Vertex3F> m_upperSurfaceBarycentricSubDivision;

	dsr::data::pathfinding::AStarStaticMeshPathfinder m_upperSurfacePathfinder;
	dsr::data::pathfinding::AStarStaticMeshPathfinder m_upperSurfaceSubDivisionPathfinder;
	dsr::data::pathfinding::AStarStaticMeshPathfinder m_upperSurfaceBarycentricSubDivisionPathfinder;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;

	void SetupPathLineEntity(
		const dsr::ecs::Entity entity,
		const std::string& name,
		const DirectX::XMMATRIX& model
	);

	std::variant<std::vector<float>, dsr::dsr_error> ConstructPath(
		const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& finish,
		const dsr::data::StaticMesh<dsr::data::Vertex3F>& mesh,
		dsr::data::pathfinding::AStarStaticMeshPathfinder& pathfinder,
		const DirectX::XMVECTORF32& color
	);

	dsr::DsrResult SetPath(
		const dsr::ecs::Entity& entity,
		const std::vector<float>& pathBuffer
	);
};
