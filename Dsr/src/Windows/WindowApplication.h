#pragma once

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/Time/FrameDelta.h"
#include "EngineSubSystems/Time/Time.h"
#include "EngineSubSystems/Input/InputSystem.h"

#include "Events/EventEmitter.h"
#include "Events/Application/WindowEvents.h"
#include "Events/Application/FrameEvents.h"

#include "Infrastructure/EventDispatcher.h"

namespace dsr
{
	namespace windows
	{
		class WindowApplication
		{
		public:
			static std::shared_ptr<WindowApplication> Get();

			EventRegisterType<const dsr::events::UpdateFrameEvent&>& GetUpdateFrameEventRegister() { return m_updateFrameEmitter; }

			std::shared_ptr<dsr::time::Time> GetTime() const { return m_time; }
			std::shared_ptr<dsr::input::InputSystem> GetInputSystem() const { return m_inputSystem; }

			std::shared_ptr<dsr::directX::Direct3dDevice> GetDevice() const { return m_device; }
			void SetDevice(const std::shared_ptr<dsr::directX::Direct3dDevice>& device) { m_device = device; }

			void RegisterEvents(std::shared_ptr<dsr::EventDispatcher> dispatcher);

			void PeekMessages();
			void ShutDown();
		private:
			WindowApplication();

			EventEmitterType<const dsr::events::UpdateFrameEvent&> m_updateFrameEmitter;

			static std::shared_ptr<WindowApplication> m_instance;
			static std::mutex m_appMutex;

			std::shared_ptr<dsr::time::Time> m_time;
			std::shared_ptr<dsr::input::InputSystem> m_inputSystem;

			std::shared_ptr<dsr::directX::Direct3dDevice> m_device;
		};
	}
}
