#include "dsrpch.h"
#include "DsrApplication.h"

namespace dsr
{
	void DsrApplication::WindowManager::OnWindowDestroy(const events::WindowDestroyEvent& destroyEvent)
	{
		using namespace dsr::windows;

		std::shared_ptr<WindowApplication> application = WindowApplication::Get();
		application->ShutDown();
	}

	DsrApplication::WindowManager::WindowManager(
		const std::shared_ptr<windows::Window>& window,
		const std::shared_ptr<directX::Direct3dDevice>& device)
		: m_window(window), m_device(device)
	{
	}

	void DsrApplication::Initialize(const int& argc, const char* const* argv)
	{
		if (argc > 0)
			m_executablePath = std::filesystem::path(argv[0]).parent_path();

		m_device = directX::Direct3dDevice::Create(m_window);
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
		m_window->GetDestroyEventRegister().Hook(m_windowManager, &DsrApplication::WindowManager::OnWindowDestroy);

		m_inputSystem->RegisterEvents(m_eventDispatcher);
		m_eventDispatcher->RegisterEventListener(m_ecsManager, &dsr::ecs::EcsManager::OnUpdate);

		DsrResult setupSystemsResult = SetupSystems();
		if (setupSystemsResult.GetResultStatusCode() != RESULT_SUCCESS)
			return setupSystemsResult;

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
		m_cameraEntity(0), m_defaultShaderProgramEntity(0), m_executablePath(std::filesystem::path(""))
	{
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
		m_cameraEntity = m_ecsManager->CreateNewEntity();
		m_defaultShaderProgramEntity = m_ecsManager->CreateNewEntity();
	}

	DsrResult DsrApplication::SetupSystems()
	{
		DsrResult initializeRendererResult = m_rendererSystem->Initialize();

		if (initializeRendererResult.GetResultStatusCode() != RESULT_SUCCESS)
			return initializeRendererResult;

		m_eventDispatcher->RegisterEventListener(m_viewProjectionSystem, &dsr::ecs::ViewProjectionSystem::HandleWindowResized);

		m_eventDispatcher->RegisterEventListener(m_rendererSystem, &dsr::ecs::RendererSystem::PrepareUpdate);
		m_eventDispatcher->RegisterEventListener(m_rendererSystem, &dsr::ecs::RendererSystem::UpdateFinished);

		m_ecsManager->RegisterSystem(m_viewProjectionSystem);
		m_ecsManager->RegisterSystem(m_rendererSystem);

		return DsrResult::Success("Setup Systems complete.");
	}

	void DsrApplication::SetupPredefinedEntities()
	{
		SetupPredefinedMainCameraEntity();
		SetupDefaultShaderProgramEntity();
	}

	void DsrApplication::SetupPredefinedMainCameraEntity()
	{
		using namespace dsr::ecs;

		std::shared_ptr<NameComponent> cameraName = m_ecsManager->RegisterComponent<NameComponent>(m_cameraEntity);
		std::shared_ptr<TagComponent> cameraTag = m_ecsManager->RegisterComponent<TagComponent>(m_cameraEntity);
		m_ecsManager->RegisterComponent<TransformComponent>(m_cameraEntity);
		m_ecsManager->RegisterComponent<ViewFrustumComponent>(m_cameraEntity);
		m_ecsManager->RegisterComponent<ViewProjectionComponent>(m_cameraEntity);

		cameraName->SetName("MainCamera");
		cameraTag->SetTag("Camera");
	}

	void DsrApplication::SetupDefaultShaderProgramEntity()
	{
		using namespace dsr::directX;
		using namespace dsr::ecs;

		Direct3dShaderInputLayout vertexShaderInputLayout;
		vertexShaderInputLayout.AddVector3f("POSITION");
		vertexShaderInputLayout.AddVector2f("TXCOORD");
		vertexShaderInputLayout.AddVector3f("NORMAL");

		std::variant<std::shared_ptr<directX::Direct3dShader<ID3D11VertexShader>>, dsr_error> loadDefaultVertexShaderResult
			= directX::LoadPrecompiledShader<ID3D11VertexShader>(m_device, m_executablePath / "DefaultVertexShader.cso");

		if (std::holds_alternative<dsr_error>(loadDefaultVertexShaderResult))
		{
			const dsr_error& error = std::get<dsr_error>(loadDefaultVertexShaderResult);
			std::string errorMessage = "Error loading default Vertexshader. ";
			errorMessage += error.what();

			std::cout << errorMessage << std::endl;
			return;
		}

		std::shared_ptr<directX::Direct3dShader<ID3D11VertexShader>> defaultVertexShader = std::get<std::shared_ptr<directX::Direct3dShader<ID3D11VertexShader>>>(loadDefaultVertexShaderResult);

		std::variant<std::shared_ptr<directX::Direct3dShader<ID3D11PixelShader>>, dsr_error> loadDefaultPixelShaderResult
			= directX::LoadPrecompiledShader<ID3D11PixelShader>(m_device, m_executablePath / "DefaultPixelShader.cso");

		if (std::holds_alternative<dsr_error>(loadDefaultPixelShaderResult))
		{
			const dsr_error& error = std::get<dsr_error>(loadDefaultPixelShaderResult);
			std::string errorMessage = "Error loading default Pixelshader. ";
			errorMessage += error.what();

			std::cout << errorMessage << std::endl;
			return;
		}

		std::shared_ptr<directX::Direct3dShader<ID3D11PixelShader>> defaultPixelShader = std::get<std::shared_ptr<directX::Direct3dShader<ID3D11PixelShader>>>(loadDefaultPixelShaderResult);

		std::variant<std::shared_ptr<Direct3dShaderProgram>, dsr_error> loadDefaultShaderProgram = CreateShaderProgramPtr(
			m_device, defaultVertexShader, defaultPixelShader, vertexShaderInputLayout);

		if (std::holds_alternative<dsr_error>(loadDefaultShaderProgram))
		{
			const dsr_error& error = std::get<dsr_error>(loadDefaultPixelShaderResult);
			std::string errorMessage = "Error creating default Shaderprogram. ";
			errorMessage += error.what();

			std::cout << errorMessage << std::endl;
			return;
		}

		std::shared_ptr<NameComponent> defaultShaderProgramName = m_ecsManager->RegisterComponent<NameComponent>(m_defaultShaderProgramEntity);
		std::shared_ptr<TagComponent> defaultShaderProgramTag = m_ecsManager->RegisterComponent<TagComponent>(m_defaultShaderProgramEntity);
		std::shared_ptr<ShaderProgramComponent> defaultShaderProgramComponent = m_ecsManager->RegisterComponent<ShaderProgramComponent>(m_defaultShaderProgramEntity);

		defaultShaderProgramName->SetName("ShaderProgram-Default ");
		defaultShaderProgramTag->SetTag("DefaultShaderProgram");
		defaultShaderProgramComponent->SetShaderProgram(std::get<std::shared_ptr<Direct3dShaderProgram>>(loadDefaultShaderProgram));
	}
}
