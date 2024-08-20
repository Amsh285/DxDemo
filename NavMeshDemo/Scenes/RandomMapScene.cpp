#include "RandomMapScene.h"

RandomMapScene::RandomMapScene(
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
)	: NavMeshSimulationSceneBase("RandomMap", sceneManager, device, blenderModelLoader)
{
	using namespace DirectX;

	m_sceneSettings.UpperSurfaceModel = XMMatrixTranslation(0.0f, 0.0f, -60.0f);
	m_sceneSettings.UpperSurfaceSubDivisonModel = XMMatrixTranslation(-60.0f, 0.0f, 0.0f);
	m_sceneSettings.UpperSurfaceBarycentricSubDivisionModel = XMMatrixTranslation(60.0f, 0.0f, 0.0f);
}
