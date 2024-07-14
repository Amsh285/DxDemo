#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Data/ModelConfiguration.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

#include "Scenes/Actors/NavMeshSimulationSceneBenchmarks.h"
#include "Scenes/Actors/NavMeshSimulationScenePathfinders.h"
#include "Scenes/Actors/NavMeshSimulationSceneMarkers.h"
#include "Scenes/Actors/NavMeshSimulationScenePaths.h"
#include "Scenes/Actors/NavMeshSimulationSceneMeshSubDivision.h"
#include "Scenes/Actors/Mediators/NavMeshSimulationSceneMediator.h"

#include "Scenes/Data/NavMeshSimulationSceneSettings.h"

#include "Scenes/Data/Events/EditorScreenClickEvent.h"

constexpr auto ERROR_LOAD_BASEMESH = 100;

constexpr auto ERROR_REGISTER_BASEMESH_SETUPMODEL = 200;

constexpr auto ERROR_REGISTER_UPPERSURFACE_SETUPMODEL = 300;

constexpr auto ERROR_REGISTER_SUBDIVIDED_UPPERSURFACE_SETUPMODEL = 400;

constexpr auto ERROR_REGISTER_BARYCENTRICSUBDIVIDED_UPPERSURFACE_SETUPMODEL = 500;

class NavMeshSimulationSceneBase
{
public:
	std::shared_ptr<NavMeshSimulationSceneBenchmarks> GetBenchmarks() const { return m_benchmarks; }

	uint32_t GetUpperSurfaceSubDivisionCount() const { return m_upperSurfaceSubDivision->GetSubDivisionCount(); }
	uint32_t GetUpperSurfaceBarycentricSubDivisionCount() const { return m_upperSurfaceBarycentricSubDivision->GetSubDivisionCount(); }

	uint32_t GetSceneId() const { return m_sceneId; }
	std::string GetSceneName() const { return m_sceneName; }

	NavMeshSimulationSceneSettings GetSceneSettings() const { return m_sceneSettings; }
	void SetSceneSettings(const NavMeshSimulationSceneSettings& settings) { m_sceneSettings = settings; }

	NavMeshSimulationSceneBase(
		const std::string& sceneName,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
		const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
	);

	virtual ~NavMeshSimulationSceneBase() = default;

	void SetActive();

	dsr::DsrResult BuildScene();

	void OnScreenClick(const EditorScreenClickEvent& screenClickEvent);
	dsr::DsrResult UpdateUpperSurfaceSubDivision(const uint32_t count);
	dsr::DsrResult UpdateUpperSurfaceBarycentricSubDivision(const uint32_t count);

	NavMeshSimulationSceneBenchmarkResult RunUpperSurfaceBenchmark(const uint32_t iterations);
protected:
	uint32_t m_sceneId;
	std::string m_sceneName;

	NavMeshSimulationSceneSettings m_sceneSettings;
	NavMeshSimulationSceneMediator m_sceneMediator;
	
	std::shared_ptr<NavMeshSimulationSceneMarkers> m_markers;
	std::shared_ptr<NavMeshSimulationScenePaths> m_paths;
	std::shared_ptr<NavMeshSimulationScenePathfinders> m_pathfinders;
	std::shared_ptr<NavMeshSimulationSceneBenchmarks> m_benchmarks;

	dsr::ecs::Entity m_baseMeshEntity;
	dsr::ecs::Entity m_upperSurfaceEntity;

	std::shared_ptr<dsr::WavefrontModel> m_baseMesh;
	std::shared_ptr<dsr::WavefrontModel> m_upperSurface;

	std::shared_ptr<NavMeshSimulationSceneMeshSubDivision> m_upperSurfaceSubDivision;
	std::shared_ptr<NavMeshSimulationSceneMeshSubDivision> m_upperSurfaceBarycentricSubDivision;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;
	std::shared_ptr<dsr::BlenderModelLoader> m_blenderModelLoader;
private:
	dsr::DsrResult LoadSceneData();
	dsr::DsrResult LoadBaseMesh();
	void LoadUpperSurface();

	dsr::DsrResult RegisterBaseMesh();
	dsr::DsrResult RegisterUpperSurface();
};
