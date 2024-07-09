#pragma once

#include "dsrpch.h"

#include "Scenes/Data/NavMeshSimulationSceneSettings.h"

#include "Data/Pathfinding/AStarStaticMeshPathfinder.h"
#include "Data/Pathfinding/PathPreProcessor.h"
#include "Data/Pathfinding/PathPostProcessor.h"

#include "Data/Structures/StaticMeshTriangle.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

#include "NavMeshSimulationScenePathfinders.h"

constexpr const char* PATHLINE_TAG = "Pathlinemarker";

constexpr auto ERROR_SETPATH_TRIANGLESNOTFOUND = 700;

constexpr auto ERROR_CONSTRUCTPATH_UPPERSURFACE = 800;
constexpr auto ERROR_CONSTRUCTPATH_UPPERSURFACE_SUBDIVISION = 801;
constexpr auto ERROR_CONSTRUCTPATH_UPPERSURFACE_BARYCENTRICSUBDIVISION = 802;

constexpr auto ERROR_SETPATH_SETUPD3D11BUFFER = 900;

class NavMeshSimulationScenePaths
{
public:
	NavMeshSimulationScenePaths(
		const uint32_t& sceneId,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device
	);

	void Setup(const NavMeshSimulationSceneSettings& settings);

	dsr::DsrResult SetBaseMeshPath(const std::vector<float>& pathBuffer);
	dsr::DsrResult SetUpperSurfacePath(const std::vector<float>& pathBuffer);
	dsr::DsrResult SetUpperSurfaceSubDivisionPath(const std::vector<float>& pathBuffer);
	dsr::DsrResult SetUpperSurfaceBarycentricSubDivisionPath(const std::vector<float>& pathBuffer);
private:
	uint32_t m_sceneId;

	dsr::ecs::Entity m_baseMeshPathEntity;
	dsr::ecs::Entity m_upperSurfacePathEntity;
	dsr::ecs::Entity m_upperSurfaceSubDivisionPathEntity;
	dsr::ecs::Entity m_upperSurfaceBarycentricSubDivisionPathEntity;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;

	void SetupPathLineEntity(
		const dsr::ecs::Entity entity,
		const std::string& name,
		const DirectX::XMMATRIX& model
	);

	dsr::DsrResult SetPath(
		const dsr::ecs::Entity& entity,
		const std::vector<float>& pathBuffer
	);
};
