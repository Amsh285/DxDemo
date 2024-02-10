#pragma once

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"

#include "Windows/Window.h"
#include "Windows/WindowApplication.h"
#include "Windows/Input/WinInput.h"

#include "ModelLoaders/BlenderModelLoader.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"

#include "DirectX/Direct3dDeviceBufferExtensions.h"
#include "DirectX/Direct3dDeviceShaderExtensions.h"

#include "Infrastructure/EventDispatcher.h"

#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TagComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ShaderProgramComponent.h"

#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"
#include "EngineSubSystems/EntityComponentSystem/EcsSystems/ViewProjectionSystem.h"
#include "EngineSubSystems/EntityComponentSystem/EcsSystems/Renderers/LineListRendererSystem.h"
#include "EngineSubSystems/EntityComponentSystem/EcsSystems/Renderers/StaticMeshRendererSystem.h"
#include "EngineSubSystems/EntityComponentSystem/EcsSystems/Renderers/WireframeRendererSystem.h"

#include "EngineSubSystems/Input/InputSystem.h"
#include "EngineSubSystems/Time/Time.h"

namespace dsr
{
	class DsrApplication
	{
	public:
		void Initialize(const int& argc, const char* const* argv);
		virtual DsrResult Setup();
		void Run();

		DsrApplication(
			const std::wstring& title,
			const int& x, const int& y,
			const int& width, const int& height);
		virtual ~DsrApplication() = default;
	protected:
		std::shared_ptr<dsr::input::Input> GetInput() const;
		std::shared_ptr<directX::Direct3dDevice> m_device;

		std::shared_ptr<dsr::EventDispatcher> m_eventDispatcher;
		std::shared_ptr<BlenderModelLoader> m_blenderModelLoader;

		std::shared_ptr<dsr::input::InputSystem> m_inputSystem;
		std::shared_ptr<dsr::ecs::EcsManager> m_ecsManager;
		std::shared_ptr<dsr::time::Time> m_time;

		dsr::ecs::Entity m_cameraEntity;

		std::filesystem::path m_executablePath;
	private:
		class WindowManager : public events::EventListener
		{
		public:
			void OnWindowDestroy(const events::WindowDestroyEvent& destroyEvent);

			WindowManager(
				const std::shared_ptr<windows::Window>& window,
				const std::shared_ptr<directX::Direct3dDevice>& device);
			WindowManager(const WindowManager& other) = delete;
			WindowManager& operator=(const WindowManager& other) = delete;
		private:
			std::shared_ptr<windows::Window> m_window;
			std::shared_ptr<directX::Direct3dDevice> m_device;
		};

		void InitializeSystems();
		DsrResult SetupSystems();
		void SetupPredefinedEntities();
		void SetupPredefinedMainCameraEntity();
		void SetupDefaultShaderProgramEntity();
		void SetupLineListShaderProgramEntity();

		std::shared_ptr<windows::Window> m_window;
		std::shared_ptr<windows::WindowApplication> m_windowApplication;
		std::shared_ptr<WindowManager> m_windowManager;

		std::shared_ptr<dsr::ecs::ViewProjectionSystem> m_viewProjectionSystem;
		std::shared_ptr<dsr::ecs::StaticMeshRendererSystem> m_staticMeshRendererSystem;
		std::shared_ptr<dsr::ecs::LineListRendererSystem> m_lineListRendererSystem;
		std::shared_ptr<dsr::ecs::WireframeRendererSystem> m_wireframeRendererSystem;

		dsr::ecs::Entity m_defaultShaderProgramEntity;
		dsr::ecs::Entity m_lineListShaderProgramEntity;
	};
}
