#pragma once

#include "dsrpch.h"

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ModelLoaders/BlenderModelLoader.h"

#include "Scenes/Data/NavMeshSimulationSceneSettings.h"
#include "Scenes/Data/PathSelectType.h"

constexpr const char* MARKER_TAG = "Pathmarker";

constexpr auto ERROR_REGISTER_MARKER_SETUPMODEL = 600;

constexpr auto ERROR_UPDATEMARKERPOSITION_COMPONENTMISSING = 700;

class NavMeshSimulationSceneMarkers
{
public:
	DirectX::XMVECTOR GetStartPositionLocal() const { return m_startPositionLocal; }
	DirectX::XMVECTOR GetFinishPositionLocal() const { return m_finishPositionLocal; }

	NavMeshSimulationSceneMarkers(
		const uint32_t& sceneId,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device
	);

	dsr::DsrResult SetupMarkers(
		const NavMeshSimulationSceneSettings& settings,
		const std::shared_ptr<dsr::WavefrontModel>& baseMesh
	);

	dsr::DsrResult SetMarkerPositions(const PathSelectType& type, const DirectX::XMVECTOR& newPosition);
private:
	uint32_t m_sceneId;

	DirectX::XMVECTOR m_startPositionLocal;
	DirectX::XMVECTOR m_finishPositionLocal;

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

	dsr::DsrResult UpdateMarkerPosition(
		const dsr::ecs::Entity& entity,
		const PathSelectType& type,
		const DirectX::XMVECTOR& newPosition,
		const float& markerLineOffset = 3.0f
	);
};
