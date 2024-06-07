#include "NavMeshSimulationSceneMarkers.h"

#include "EngineSubSystems/EntityComponentSystem/Components/LineListComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TagComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"

#include "Infrastructure/XMathHelper.h"


NavMeshSimulationSceneMarkers::NavMeshSimulationSceneMarkers(
	const uint32_t& sceneId,
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device
) : m_sceneId(sceneId), m_sceneManager(sceneManager), m_device(device)
{
	m_startColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	m_finishColor = { 1.0f, 0.0f, 0.0f, 1.0f };

	m_baseMeshMarkersEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceMarkersEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceSubDivisionMarkersEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceBarycentricSubDivisionMarkersEntity = m_sceneManager->CreateNewEntity();
}

dsr::DsrResult NavMeshSimulationSceneMarkers::SetupMarkers(
	const NavMeshSimulationSceneSettings& settings,
	const std::shared_ptr<dsr::WavefrontModel>& baseMesh
)
{
	using namespace dsr;
	using namespace dsr::data;

	using namespace DirectX;

	const std::vector<Vertex3FP2FTx3FN>& vertexBuffer = baseMesh->Mesh->GetVertexBuffer();
	const std::vector<uint32_t>& indexBuffer = baseMesh->Mesh->GetIndexBuffer();

	XMVECTOR startPosition = dsr::Barycenter(
		XMLoadFloat3(&vertexBuffer[indexBuffer[0]].Position),
		XMLoadFloat3(&vertexBuffer[indexBuffer[1]].Position),
		XMLoadFloat3(&vertexBuffer[indexBuffer[2]].Position)
	);

	XMVECTOR finishPosition = dsr::Barycenter(
		XMLoadFloat3(&vertexBuffer[indexBuffer[indexBuffer.size() - 3]].Position),
		XMLoadFloat3(&vertexBuffer[indexBuffer[indexBuffer.size() - 2]].Position),
		XMLoadFloat3(&vertexBuffer[indexBuffer[indexBuffer.size() - 1]].Position)
	);

	std::vector<float> vertexData = BuildMarkerVertexBuffer(startPosition, finishPosition);

	DsrResult registerBaseMeshMarkersResult = RegisterMarkers(
		m_baseMeshMarkersEntity, "Basemesh_Markers",
		vertexData,
		settings.BaseMeshModel, startPosition, finishPosition
	);

	if (registerBaseMeshMarkersResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerBaseMeshMarkersResult;

	DsrResult registerUpperSurfaceMarkersResult = RegisterMarkers(
		m_upperSurfaceMarkersEntity, "UpperSurface_Markers",
		vertexData,
		settings.UpperSurfaceModel, startPosition, finishPosition
	);

	if (registerUpperSurfaceMarkersResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerUpperSurfaceMarkersResult;

	DsrResult registerUpperSurfaceSubdivisionMarkersResult = RegisterMarkers(
		m_upperSurfaceSubDivisionMarkersEntity, "UpperSurface_SubdivisionMarkers",
		vertexData,
		settings.UpperSurfaceSubDivisonModel, startPosition, finishPosition
	);

	if (registerUpperSurfaceSubdivisionMarkersResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerUpperSurfaceSubdivisionMarkersResult;

	DsrResult registerUpperSurfaceBarycentricSubdivisionMarkersResult = RegisterMarkers(
		m_upperSurfaceBarycentricSubDivisionMarkersEntity, "UpperSurface_BarycentricSubdivisionMarkers",
		vertexData,
		settings.UpperSurfaceBarycentricSubDivisionModel, startPosition, finishPosition
	);

	if (registerUpperSurfaceBarycentricSubdivisionMarkersResult.GetResultStatusCode() != RESULT_SUCCESS)
		return registerUpperSurfaceBarycentricSubdivisionMarkersResult;

	return DsrResult::Success("Setup Markers Success.");
}

std::vector<float> NavMeshSimulationSceneMarkers::BuildMarkerVertexBuffer(
	const DirectX::XMVECTOR& startPosition,
	const DirectX::XMVECTOR& finishPosition,
	const float& markerLineOffset
) const
{
	using namespace DirectX;

	XMVECTOR startPositionUp = XMVectorAdd(startPosition, XMVectorSet(0.0f, markerLineOffset, 0.0f, 0.0f));
	XMVECTOR startPositionDown = XMVectorAdd(startPosition, XMVectorSet(0.0f, -markerLineOffset, 0.0f, 0.0f));

	XMVECTOR finishPositionUp = XMVectorAdd(finishPosition, XMVectorSet(0.0f, markerLineOffset, 0.0f, 0.0f));
	XMVECTOR finishPositionDown = XMVectorAdd(finishPosition, XMVectorSet(0.0f, -markerLineOffset, 0.0f, 0.0f));

	std::vector<float> vertexBuffer;
	vertexBuffer.push_back(XMVectorGetX(startPositionUp));
	vertexBuffer.push_back(XMVectorGetY(startPositionUp));
	vertexBuffer.push_back(XMVectorGetZ(startPositionUp));

	vertexBuffer.push_back(XMVectorGetX(m_startColor));
	vertexBuffer.push_back(XMVectorGetY(m_startColor));
	vertexBuffer.push_back(XMVectorGetZ(m_startColor));
	vertexBuffer.push_back(XMVectorGetW(m_startColor));

	vertexBuffer.push_back(XMVectorGetX(startPositionDown));
	vertexBuffer.push_back(XMVectorGetY(startPositionDown));
	vertexBuffer.push_back(XMVectorGetZ(startPositionDown));

	vertexBuffer.push_back(XMVectorGetX(m_startColor));
	vertexBuffer.push_back(XMVectorGetY(m_startColor));
	vertexBuffer.push_back(XMVectorGetZ(m_startColor));
	vertexBuffer.push_back(XMVectorGetW(m_startColor));

	vertexBuffer.push_back(XMVectorGetX(finishPositionUp));
	vertexBuffer.push_back(XMVectorGetY(finishPositionUp));
	vertexBuffer.push_back(XMVectorGetZ(finishPositionUp));

	vertexBuffer.push_back(XMVectorGetX(m_finishColor));
	vertexBuffer.push_back(XMVectorGetY(m_finishColor));
	vertexBuffer.push_back(XMVectorGetZ(m_finishColor));
	vertexBuffer.push_back(XMVectorGetW(m_finishColor));

	vertexBuffer.push_back(XMVectorGetX(finishPositionDown));
	vertexBuffer.push_back(XMVectorGetY(finishPositionDown));
	vertexBuffer.push_back(XMVectorGetZ(finishPositionDown));

	vertexBuffer.push_back(XMVectorGetX(m_finishColor));
	vertexBuffer.push_back(XMVectorGetY(m_finishColor));
	vertexBuffer.push_back(XMVectorGetZ(m_finishColor));
	vertexBuffer.push_back(XMVectorGetW(m_finishColor));

	return vertexBuffer;
}

dsr::DsrResult NavMeshSimulationSceneMarkers::RegisterMarkers(
	const dsr::ecs::Entity& entity,
	const std::string& name,
	const std::vector<float>& vertexData,
	const DirectX::XMMATRIX& model,
	const DirectX::XMVECTOR& startPosition,
	const DirectX::XMVECTOR& finishPosition
)
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::ecs;

	using namespace DirectX;

	std::variant<Direct3dBuffer, dsr_error> createBufferResult = Direct3dBuffer::CreateVertexBufferf(
		m_device, vertexData,
		D3D11_CPU_ACCESS_WRITE, 0, D3D11_USAGE_DYNAMIC
	);

	if (std::holds_alternative<dsr_error>(createBufferResult))
	{
		const dsr_error& error = std::get<dsr_error>(createBufferResult);
		return DsrResult(error.what(), ERROR_REGISTER_MARKER_SETUPMODEL);
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	m_sceneManager->AddComponent<NameComponent>(m_sceneId, entity, name);
	m_sceneManager->AddComponent<TagComponent>(m_sceneId, entity, MARKER_TAG);

	std::shared_ptr<LineListComponent> lineListComponent = m_sceneManager
		->AddComponent<LineListComponent>(m_sceneId, entity);

	lineListComponent->SetStartVertexLocation(0);
	lineListComponent->SetVertexCount(vertexData.size() / 7);
	lineListComponent->SetVertexShaderInputLayout(inputLayout);
	lineListComponent->SetVertexBuffer(std::get<Direct3dBuffer>(createBufferResult));

	std::shared_ptr<TransformComponent> transformComponent = m_sceneManager->AddComponent<TransformComponent>(m_sceneId, entity);

	XMVECTOR scale, rotationQuaternion, transform;
	XMMatrixDecompose(&scale, &rotationQuaternion, &transform, model);
	transformComponent->SetScale(scale);
	transformComponent->SetRotation(rotationQuaternion);
	transformComponent->SetPosition(transform);

	return DsrResult::Success("Register Marker Success.");
}

dsr::DsrResult NavMeshSimulationSceneMarkers::UpdateMarkerPosition(
	const dsr::ecs::Entity& entity,
	const MarkerType& type,
	const DirectX::XMVECTOR& newPosition,
	const float& markerLineOffset)
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::ecs;

	using namespace DirectX;

	std::shared_ptr<LineListComponent> lineListComponent = m_sceneManager
		->GetComponentFrom<LineListComponent>(m_sceneId, entity);

	if (!lineListComponent)
		return DsrResult("LineListComponent not found for Entity: " + entity, ERROR_UPDATEMARKERPOSITION_COMPONENTMISSING);

	Direct3dBuffer vertexBuffer = lineListComponent->GetVertexBuffer();

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	DsrResult mapToDeviceResult = m_device->Map(vertexBuffer.GetBufferPtr().get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	if (mapToDeviceResult.GetResultStatusCode() != RESULT_SUCCESS)
		return mapToDeviceResult;



	return DsrResult::Success("Update Markerposition Success.");
}
