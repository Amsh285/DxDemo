#include "NavMeshSimulationSceneMeshSubDivision.h"

#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/WireframeMeshComponent.h"

#include "Data/Manipulation/StaticMeshExtensions.h"
#include "Data/Manipulation/StaticMeshVertexBufferExtensions.h"

#include "DirectX/Direct3dDeviceBufferExtensions.h"

DirectX::XMMATRIX NavMeshSimulationSceneMeshSubDivision::GetModelMatrix() const
{
	using namespace dsr::ecs;

	return m_sceneManager->GetComponentFrom<TransformComponent>(m_sceneId, m_entity)->GetModelMatrix();
}

void NavMeshSimulationSceneMeshSubDivision::SetModelMatrix(const DirectX::XMMATRIX& modelMatrix)
{
	using namespace dsr::ecs;

	using namespace DirectX;

	std::shared_ptr<TransformComponent> transformComponent = m_sceneManager->GetComponentFrom<TransformComponent>(m_sceneId, m_entity);

	XMVECTOR scale, rotationQuaternion, transform;
	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, modelMatrix);
	transformComponent->SetScale(scale);
	transformComponent->SetRotation(rotationQuaternion);
	transformComponent->SetPosition(transform);
}

NavMeshSimulationSceneMeshSubDivision::NavMeshSimulationSceneMeshSubDivision(
	const uint32_t sceneId,
	const dsr::data::StaticMesh<dsr::data::Vertex3FP2FTx3FN>& baseMesh,
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device
) : m_sceneId(sceneId), m_baseMesh(baseMesh), m_sceneManager(sceneManager), m_device(device),
	m_subDivisionCount(0)
{
	m_entity = m_sceneManager->CreateNewEntity();
	m_sceneManager->AddComponent<dsr::ecs::TransformComponent>(m_sceneId, m_entity);
}

dsr::DsrResult NavMeshSimulationSceneMeshSubDivision::SubDivide(const uint32_t count)
{
	
	m_subDividedMesh = m_baseMesh;

	for (uint32_t i = 0; i < count; i++)
	{
		m_subDividedMesh = dsr::data::manipulation::SubDivide(m_subDividedMesh);
	}


	m_subDivisionCount = count;
	return UpdateScene();
}

dsr::DsrResult NavMeshSimulationSceneMeshSubDivision::SubDivideBarycentric(const uint32_t count)
{
	m_subDivisionCount = count;
	m_subDividedMesh = m_baseMesh;

	for (uint32_t i = 0; i < count; i++)
	{
		m_subDividedMesh = dsr::data::manipulation::SubDivideBarycentric(m_subDividedMesh);
	}

	return UpdateScene();
}

dsr::DsrResult NavMeshSimulationSceneMeshSubDivision::UpdateScene()
{
	using namespace dsr;

	using namespace dsr::data::manipulation;
	
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	using namespace dsr::ecs;

	using namespace DirectX;

	m_sceneManager->RemoveComponent<WireframeMeshComponent>(m_sceneId, m_entity);

	const std::vector<float> vertexBuffer = GetVertexBufferF(m_subDividedMesh);
	const std::vector<uint32_t>& indexBuffer = m_subDividedMesh.GetIndexBuffer();

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector2f("TXCOORD");
	inputLayout.AddVector3f("NORMAL");

	std::variant<Direct3dVertexBufferf, dsr_error> setupVertexBufferResult = SetupVertexBufferf(
		m_device, vertexBuffer, indexBuffer, inputLayout
	);

	if(std::holds_alternative<dsr_error>(setupVertexBufferResult))
		return DsrResult(std::get<dsr_error>(setupVertexBufferResult).what(), ERROR_UPDATESCENE_SETUPVERTEXBUFFER);

	std::shared_ptr<VertexGroup> vertexGroup = std::make_shared<VertexGroup>();
	vertexGroup->StartIndexLocation = 0;
	vertexGroup->IndexCount = indexBuffer.size();
	vertexGroup->PSData.SpecularColor = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	vertexGroup->PSData.DiffuseColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	std::shared_ptr<StaticMeshComponent> mesh = std::make_shared<StaticMeshComponent>();
	mesh->SetVertexBuffer(std::get<Direct3dVertexBufferf>(setupVertexBufferResult));
	mesh->SetVertexGroups({ vertexGroup });

	std::shared_ptr<WireframeMeshComponent> wireframeMeshComponent = m_sceneManager->AddComponent<WireframeMeshComponent>(m_sceneId, m_entity);
	wireframeMeshComponent->SetMesh(mesh);

	return DsrResult::Success("Update Scene Success.");
}
