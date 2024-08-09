#include "TerrainScene.h"

TerrainScene::TerrainScene(
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
)	: NavMeshSimulationSceneBase("Terrain", sceneManager, device, blenderModelLoader)
{
	using namespace DirectX;

	m_sceneSettings.UpperSurfaceModel = XMMatrixTranslation(0.0f, 30.0f, 0.0f);
	m_sceneSettings.UpperSurfaceSubDivisonModel = XMMatrixTranslation(-30.0f, 0.0f, 0.0f);
	m_sceneSettings.UpperSurfaceBarycentricSubDivisionModel = XMMatrixTranslation(30.0f, 0.0f, 0.0f);
}
