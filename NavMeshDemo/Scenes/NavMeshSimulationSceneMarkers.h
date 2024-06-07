#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

#include "Scenes/Data/NavMeshSimulationSceneSettings.h"

constexpr const char* MARKER_TAG = "Pathmarker";

constexpr auto ERROR_REGISTER_MARKER_SETUPMODEL = 600;

class NavMeshSimulationSceneMarkers
{
public:
	NavMeshSimulationSceneMarkers(
		const uint32_t& sceneId,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device
	);

	dsr::DsrResult SetupMarkers(
		const NavMeshSimulationSceneSettings& settings,
		const std::shared_ptr<dsr::WavefrontModel>& baseMesh
	);
private:
	uint32_t m_sceneId;

	DirectX::XMVECTORF32 m_startColor;
	DirectX::XMVECTORF32 m_finishColor;

	dsr::ecs::Entity m_baseMeshMarkersEntity;
	dsr::ecs::Entity m_upperSurfaceMarkersEntity;
	dsr::ecs::Entity m_upperSurfaceSubDivisionMarkersEntity;
	dsr::ecs::Entity m_upperSurfaceBarycentricSubDivisionMarkersEntity;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;

	std::vector<float> BuildMarkerVertexBuffer(
		const DirectX::XMVECTOR& startPosition,
		const DirectX::XMVECTOR& finishPosition,
		const float& markerLineOffset = 3.0f
	) const;

	dsr::DsrResult RegisterMarkers(
		const dsr::ecs::Entity& entity,
		const std::string& name,
		const std::vector<float>& vertexData,
		const DirectX::XMMATRIX& model,
		const DirectX::XMVECTOR& startPosition,
		const DirectX::XMVECTOR& finishPosition
	);
};
