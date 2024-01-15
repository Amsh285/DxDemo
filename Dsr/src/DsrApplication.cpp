#include "dsrpch.h"
#include "DsrApplication.h"
#include <EngineSubSystems/EntityComponentSystem/Components/NameComponent.h>
#include <EngineSubSystems/EntityComponentSystem/Components/TagComponent.h>

namespace dsr
{
	void DsrApplication::WindowManager::OnWindowDestroy(const events::WindowDestroyEvent& destroyEvent)
	{
		using namespace dsr::windows;

		std::shared_ptr<WindowApplication> application = WindowApplication::Get();
		application->ShutDown();
	}

	void DsrApplication::WindowManager::OnWindowResize(const events::WindowResizedEvent& resizeEvent)
	{
		float aspectRatio = static_cast<float>(m_window->GetClientWidth()) / static_cast<float>(m_window->GetClientHeight());
		for (std::shared_ptr<camerasystem::Camera> camera : Cameras)
			camera->AspectRatio = aspectRatio;
	}

	DsrApplication::WindowManager::WindowManager(
		const std::shared_ptr<windows::Window>& window,
		const std::shared_ptr<directX::Direct3dDevice>& device)
		: m_window(window), m_device(device)
	{
	}

	void DsrApplication::Initialize()
	{
		m_device = directX::Direct3dDevice::Create(m_window);
		m_renderer = std::make_shared<directX::rendering::Direct3dRenderer>(m_device);
		m_windowManager = std::make_shared<WindowManager>(m_window, m_device);
		m_blenderModelLoader = std::make_shared<BlenderModelLoader>();
		m_eventDispatcher = std::make_shared<dsr::EventDispatcher>(m_window, m_windowApplication);
		m_inputSystem = std::make_shared<dsr::input::InputSystem>(dsr::windows::CreateKeyMap());

		m_ecsManager = std::make_shared<dsr::ecs::EcsManager>();

		InitializeSystems();
		InitializePredefinedEntities();
	}

	DsrResult DsrApplication::Setup()
	{
		DsrResult renderInitResult = m_renderer->Initialize();
		if (renderInitResult.GetResultStatusCode() != RESULT_SUCCESS)
			return renderInitResult;

		m_mainCamera = CreateCamera();
		SetActiveCamera(m_mainCamera);

		m_window->GetDestroyEventRegister().Hook(m_windowManager, &DsrApplication::WindowManager::OnWindowDestroy);
		m_window->GetResizedEventRegister().Hook(m_windowManager, &DsrApplication::WindowManager::OnWindowResize);
		//m_windowApplication->GetUpdateFrameEventRegister().Hook(m_renderer, &directX::rendering::Direct3dRenderer::OnUpdate);

		m_inputSystem->RegisterEvents(m_eventDispatcher);

		m_eventDispatcher->RegisterEventListener(m_ecsManager, &dsr::ecs::EcsManager::OnUpdate);
		
		SetupSystems();
		SetupPredefinedEntities();

		return DsrResult::Success("base setup complete.");
	}

	void DsrApplication::Run()
	{
		m_window->Show();
		m_windowApplication->PeekMessages();
	}

	DsrApplication::DsrApplication(
		const std::wstring& title,
		const int& x, const int& y,
		const int& width, const int& height)
		: m_window(std::make_shared<windows::Window>(windows::WindowData(title, x, y, width, height))),
		m_windowApplication(windows::WindowApplication::Get()),
		m_CameraEntity(0)
	{
	}

	std::shared_ptr<camerasystem::Camera> DsrApplication::CreateCamera()
	{
		std::shared_ptr<camerasystem::Camera> camera = std::make_shared<camerasystem::Camera>();

		float aspectRatio = static_cast<float>(m_window->GetClientWidth()) / static_cast<float>(m_window->GetClientHeight());
		camera->AspectRatio = aspectRatio;

		m_windowManager->Cameras.push_back(camera);
		return camera;
	}

	void DsrApplication::SetActiveCamera(const std::shared_ptr<camerasystem::Camera>& camera)
	{
		m_renderer->SetActiveCamera(camera);
	}

	std::shared_ptr<dsr::input::Input> DsrApplication::GetInput() const
	{
		return m_inputSystem->GetInput();
	}

	void DsrApplication::InitializeSystems()
	{
		m_viewProjectionSystem = std::make_shared<dsr::ecs::ViewProjectionSystem>();
		m_rendererSystem = std::make_shared<dsr::ecs::RendererSystem>(m_device);
	}

	void DsrApplication::InitializePredefinedEntities()
	{
		m_CameraEntity = m_ecsManager->CreateNewEntity();
	}

	void DsrApplication::SetupSystems()
	{
		m_eventDispatcher->RegisterEventListener(m_viewProjectionSystem, &dsr::ecs::ViewProjectionSystem::HandleWindowResized);
		m_eventDispatcher->RegisterEventListener(m_rendererSystem, &dsr::ecs::RendererSystem::PrepareUpdate);

		m_ecsManager->RegisterSystem(m_viewProjectionSystem);
		m_ecsManager->RegisterSystem(m_rendererSystem);
	}

	void DsrApplication::SetupPredefinedEntities()
	{
		using namespace dsr::ecs;

		std::shared_ptr<NameComponent> cameraName = m_ecsManager->RegisterComponent<NameComponent>(m_CameraEntity);
		std::shared_ptr<TagComponent> cameraTag = m_ecsManager->RegisterComponent<TagComponent>(m_CameraEntity);
		m_ecsManager->RegisterComponent<TransformComponent>(m_CameraEntity);
		m_ecsManager->RegisterComponent<ViewFrustumComponent>(m_CameraEntity);
		m_ecsManager->RegisterComponent<ViewProjectionComponent>(m_CameraEntity);

		cameraName->SetName("MainCamera");
		cameraTag->SetTag("Camera");
	}
}
