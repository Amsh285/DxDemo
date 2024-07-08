#include "BridgeScene.h"

#include "DirectX/Rendering/ModelLoaderExtensions/BlenderModelLoaderExtensions.h"
#include "Data/Manipulation/StaticMeshExtensions.h"
#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"

BridgeScene::BridgeScene(
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device,
	const std::shared_ptr<dsr::BlenderModelLoader>& blenderModelLoader
)	: NavMeshSimulationSceneBase("Bridges", sceneManager, device, blenderModelLoader)
{
	using namespace DirectX;

	m_sceneSettings.UpperSurfaceModel = XMMatrixTranslation(0.0f, 40.0f, 0.0f);
	m_sceneSettings.UpperSurfaceSubDivisonModel = XMMatrixTranslation(-80.0f, 0.0f, 0.0f);
	m_sceneSettings.UpperSurfaceBarycentricSubDivisionModel = XMMatrixTranslation(80.0f, 0.0f, 0.0f);
}
