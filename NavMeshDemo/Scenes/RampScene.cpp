#include "RampScene.h"

#include "Data/Manipulation/StaticMeshExtensions.h"

RampScene::RampScene(
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
)	: NavMeshSimulationSceneBase("Ramp", sceneManager, device, blenderModelLoader)
{
	using namespace DirectX;

	m_sceneSettings.UpperSurfaceModel = XMMatrixTranslation(0.0f, 40.0f, 0.0f);
	m_sceneSettings.UpperSurfaceSubDivisonModel = XMMatrixTranslation(-80.0f, 0.0f, 0.0f);
	m_sceneSettings.UpperSurfaceBarycentricSubDivisionModel = XMMatrixTranslation(80.0f, 0.0f, 0.0f);
}

void RampScene::LogTest() const
{
	using namespace dsr::data::manipulation;

	m_baseMesh->Mesh->GetAdjacencyListSequential();

	auto adjacencyList = m_pathfinders->GetUpperSurfacePathfinder()->GetAdjacencyList();

	LogAdjacencyList("upperSurface sequential", adjacencyList);
	LogAdjacencyList("upperSurface parallel", adjacencyList);
}

//void RampScene::RegisterMapFaceNormalsEntity()
//{
//	using namespace dsr;
//	using namespace dsr::data;
//	using namespace dsr::directX;
//	using namespace dsr::directX::rendering;
//
//	std::vector<Face> mapFaces = GetFaceData(m_baseMesh);
//	std::vector<float> vertexData;
//
//	for (const Face& face : mapFaces)
//	{
//		vertexData.push_back(face.Centroid.x);
//		vertexData.push_back(face.Centroid.y);
//		vertexData.push_back(face.Centroid.z);
//
//		vertexData.push_back(0.0f);
//		vertexData.push_back(1.0f);
//		vertexData.push_back(0.0f);
//		vertexData.push_back(1.0f);
//
//		vertexData.push_back(face.Centroid.x + face.Normal.x);
//		vertexData.push_back(face.Centroid.y + face.Normal.y);
//		vertexData.push_back(face.Centroid.z + face.Normal.z);
//
//		vertexData.push_back(0.0f);
//		vertexData.push_back(1.0f);
//		vertexData.push_back(0.0f);
//		vertexData.push_back(1.0f);
//	}
//
//	std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(m_device, vertexData);
//	if (std::holds_alternative<dsr_error>(createVertexBuffer))
//	{
//		const dsr_error& err = std::get<dsr_error>(createVertexBuffer);
//		std::cout << "error creatingVertexBuffer for MapFaceNormals: " << err.what() << std::endl;
//		return;
//	}
//
//	Direct3dShaderInputLayout inputLayout;
//	inputLayout.AddVector3f("POSITION");
//	inputLayout.AddVector4f("COLOR");
//
//	std::shared_ptr<dsr::ecs::LineListComponent> lines = m_sceneManager->AddComponent<dsr::ecs::LineListComponent>(m_sceneId, m_mapFaceNormalsEntity);
//	lines->SetVertexCount(mapFaces.size() * 2);
//	lines->SetVertexBuffer(std::get<Direct3dBuffer>(createVertexBuffer));
//	lines->SetVertexShaderInputLayout(inputLayout);
//}
