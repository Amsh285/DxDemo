#pragma once

#include "dsrpch.h"

#include "Data/Structures/StaticMesh.h"
#include "Data/Vertex.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ErrorHandling/dsr_error.h"
#include "ErrorHandling/DsrResult.h"

constexpr auto ERROR_UPDATESCENE_SETUPVERTEXBUFFER = 600;

class NavMeshSimulationSceneMeshSubDivision
{
public:
	uint32_t GetSceneId() const { return m_sceneId; }
	dsr::ecs::Entity GetEntity() const { return m_entity; }

	uint32_t GetSubDivisionCount() const { return m_subDivisionCount; }

	DirectX::XMMATRIX GetModelMatrix() const;
	void SetModelMatrix(const DirectX::XMMATRIX& modelMatrix);

	const dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN>& GetBaseMesh() const { return m_baseMesh; }
	const dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN>& GetSubDividedMesh() const { return m_subDividedMesh; }

	NavMeshSimulationSceneMeshSubDivision(
		const uint32_t sceneId,
		const dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN>& baseMesh,
		const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
		const std::shared_ptr<dsr::directX::Direct3dDevice>& device
	);

	dsr::DsrResult SubDivide(const uint32_t count);
	dsr::DsrResult SubDivideBarycentric(const uint32_t count);
private:
	uint32_t m_sceneId;
	dsr::ecs::Entity m_entity;

	uint32_t m_subDivisionCount;

	dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN> m_baseMesh;
	dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN> m_subDividedMesh;

	std::shared_ptr<dsr::scene::SceneManager> m_sceneManager;
	std::shared_ptr<dsr::directX::Direct3dDevice> m_device;

	dsr::DsrResult UpdateScene();
};
