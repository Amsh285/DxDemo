#include "NavMeshSimulationScenePaths.h"

#include "Data/Manipulation/StaticMeshExtensions.h"

NavMeshSimulationScenePaths::NavMeshSimulationScenePaths(
	const uint32_t& sceneId,
	const std::shared_ptr<dsr::scene::SceneManager>& sceneManager,
	const std::shared_ptr<dsr::directX::Direct3dDevice>& device
) : m_sceneId(sceneId), m_sceneManager(sceneManager), m_device(device)
{
	m_baseMeshPathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfacePathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceSurfaceSubDivisionPathEntity = m_sceneManager->CreateNewEntity();
	m_upperSurfaceBarycentricSubDivisionPathEntity = m_sceneManager->CreateNewEntity();
}

void NavMeshSimulationScenePaths::Setup(
	const DirectX::XMVECTOR& start,
	const DirectX::XMVECTOR& finish,
	std::shared_ptr<dsr::WavefrontModel> baseMesh,
	std::shared_ptr<dsr::WavefrontModel> upperSurface,
	std::shared_ptr<dsr::WavefrontModel> upperSurfaceSubDivision,
	std::shared_ptr<dsr::WavefrontModel> upperSurfaceBarycentricSubDivision
)
{
	using namespace dsr;
	using namespace dsr::data::manipulation;

	m_start = start;
	m_finish = finish;

	m_baseMesh = FilterDistinct(*baseMesh->Mesh);
	m_upperSurface = FilterDistinct(*upperSurface->Mesh);
	m_upperSurfaceSubDivision = FilterDistinct(*upperSurfaceSubDivision->Mesh);
	m_upperSurfaceBarycentricSubDivision = FilterDistinct(*upperSurfaceBarycentricSubDivision->Mesh);

	m_baseMeshPathfinder.SetGraph(m_baseMesh);
	m_upperSurfacePathfinder.SetGraph(m_upperSurface);
	m_upperSurfaceSurfaceSubDivisionPathfinder.SetGraph(m_upperSurfaceSubDivision);
	m_upperSurfaceBarycentricSubDivisionPathfinder.SetGraph(m_upperSurfaceBarycentricSubDivision);
}
