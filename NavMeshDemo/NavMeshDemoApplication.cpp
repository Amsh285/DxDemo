#include "NavMeshDemoApplication.h"


NavMeshDemoApplication::NavMeshDemoApplication()
	: DsrApplication(L"Nav Mesh Demo", 100, 100, 1280, 768)
{
	m_mapEntity = dsr::ecs::EcsManager::CreateNewEntity();
}

std::variant<dsr::ModelConfiguration, dsr::dsr_error> NavMeshDemoApplication::LoadMapModel()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	std::variant<ModelConfiguration, dsr_error> loadMapResult = LoadWavefrontModel(
		m_device,
		m_blenderModelLoader,
		"Assets/",
		"map.wf",
		"map.mtl");

	if (std::holds_alternative<dsr_error>(loadMapResult))
		return std::get<dsr_error>(loadMapResult);

	return std::get<ModelConfiguration>(loadMapResult);
}

void NavMeshDemoApplication::RegisterMapModel(const dsr::ModelConfiguration& map)
{
	using namespace dsr::ecs;

	m_ecsManager->RegisterComponent<NameComponent>(m_mapEntity, "map");
	m_ecsManager->RegisterComponent<TagComponent>(m_mapEntity, "map");
	m_ecsManager->RegisterComponent<TransformComponent>(m_mapEntity);
	
	std::shared_ptr<StaticMeshComponent> mesh = m_ecsManager->RegisterComponent<StaticMeshComponent>(m_mapEntity);
	mesh->SetVertexBuffer(map.GetVertexBuffer());
	mesh->SetVertexGroups(map.GetVertexGroups());
}

dsr::DsrResult NavMeshDemoApplication::Setup()
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::directX::rendering;

	dsr::DsrResult baseResult = DsrApplication::Setup();
	if (baseResult.GetResultStatusCode() != RESULT_SUCCESS)
		return baseResult;

	std::variant<ModelConfiguration, dsr_error> loadMapResult = LoadMapModel();
	if (std::holds_alternative<dsr_error>(loadMapResult))
	{
		const dsr_error& error = std::get<dsr_error>(loadMapResult);
		std::string errorMessage = "Error loading Map: ";
		errorMessage += error.what();
		return DsrResult(errorMessage, error.GetResult());
	}

	ModelConfiguration config = std::get<ModelConfiguration>(loadMapResult);
	RegisterMapModel(config);

	std::vector<dsr::ecs::Entity> cameraEntities = m_ecsManager->FindEntitiesByTag("Camera");
	std::shared_ptr<dsr::ecs::TransformComponent> cameraTransform = m_ecsManager->GetComponentFrom<dsr::ecs::TransformComponent>(cameraEntities[0]);

	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0.0f, 10.0f, -50.0f);
	cameraTransform->SetPositionVec3(cameraPosition);

	return dsr::DsrResult::Success("Setup Successful.");

	return dsr::DsrResult::Success("");
}
