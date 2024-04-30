#include "NavMeshDemoApplication.h"

NavMeshDemoApplication::NavMeshDemoApplication()
	: DsrApplication(L"Nav Mesh Demo", 100, 100, 1280, 768)
{
	using namespace dsr::ecs;

	m_lineEntity = EcsManager::CreateNewEntity();
	m_editorUIEntity = EcsManager::CreateNewEntity();
}

void NavMeshDemoApplication::RegisterLineEntity()
{
	using namespace dsr;
	using namespace dsr::data;
	using namespace dsr::directX;

	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_ecsManager->RegisterComponent<dsr::ecs::LineListComponent>(m_lineEntity);

	std::vector<Vertex3FP4FC> lineList;
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, -100.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f)));
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, 100.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f)));

	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, 0.0f, -100.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.8f, 1.0f)));
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.8f, 1.0f)));

	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(-100.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f)));
	lineList.push_back(Vertex3FP4FC(DirectX::XMFLOAT3(100.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f)));

	std::vector<float> vertexData;

	for (const Vertex3FP4FC& vertex : lineList)
	{
		vertexData.push_back(vertex.Position.x);
		vertexData.push_back(vertex.Position.y);
		vertexData.push_back(vertex.Position.z);

		vertexData.push_back(vertex.Color.x);
		vertexData.push_back(vertex.Color.y);
		vertexData.push_back(vertex.Color.z);
		vertexData.push_back(vertex.Color.w);
	}

	std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(m_device, vertexData);
	if (std::holds_alternative<dsr_error>(createVertexBuffer))
	{
		const dsr_error& err = std::get<dsr_error>(createVertexBuffer);
		std::cout << "error creatingVertexBuffer for LineData: " << err.what() << std::endl;
		return;
	}

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector4f("COLOR");

	lines->SetVertexCount(lineList.size());
	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
	lines->SetVertexShaderInputLayout(inputLayout);
}

void NavMeshDemoApplication::RegisterCameraController()
{
	m_cameraControllerSystem = std::make_shared<CameraControllerSystem>(GetInput(), GetTime());
	m_ecsManager->RegisterSystem(m_cameraControllerSystem);

	m_ecsManager->RegisterComponent<CameraControllerComponent>(m_cameraEntity);
}

void NavMeshDemoApplication::RegisterEditorUI()
{
	m_editorUISystem = std::make_shared<EditorUISystem>(GetSceneManager(), GetInput());
	m_ecsManager->RegisterSystem(m_editorUISystem);

	m_ecsManager->RegisterComponent<EditorUIComponent>(m_editorUIEntity);
}

dsr::DsrResult NavMeshDemoApplication::Setup()
{
	using namespace dsr;
	

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	m_rampScene = std::make_shared<RampScene>(m_sceneManager, m_device, m_blenderModelLoader);
	DsrResult rampSceneResult = m_rampScene->BuildScene();

	if (rampSceneResult.GetResultStatusCode() != RESULT_SUCCESS)
		return rampSceneResult;

	m_bridgeScene = std::make_shared<BridgeScene>(m_sceneManager, m_device, m_blenderModelLoader);
	DsrResult bridgeSceneResult = m_bridgeScene->BuildScene();

	if (bridgeSceneResult.GetResultStatusCode() != RESULT_SUCCESS)
		return bridgeSceneResult;

	RegisterLineEntity();
	RegisterCameraController();
	RegisterEditorUI();

	std::vector<dsr::ecs::Entity> cameraEntities = m_ecsManager->FindEntitiesByTag("Camera");
	std::shared_ptr<dsr::ecs::TransformComponent> cameraTransform = m_ecsManager->GetComponentFrom<dsr::ecs::TransformComponent>(cameraEntities[0]);

	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f);
	cameraTransform->SetPositionVec3(cameraPosition);

	m_rampScene->SetActive();
	return dsr::DsrResult::Success("Setup Successful.");
}
