#include "RampScene.h"

RampScene::RampScene(
	const std::shared_ptr<dsr::scenesystem::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
)	: m_sceneManager(sceneManager), m_device(device), m_blenderModelLoader(blenderModelLoader)
{
	m_sceneName = "Ramp";
	m_sceneId = m_sceneManager->CreateNewScene(m_sceneName);

}

void RampScene::BuildScene()
{
}
