#pragma once

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"
#include "Events/Application/InputEvents/KeyboardEvents.h"
#include "Events/Application/InputEvents/MouseEvents.h"

// this should be refactored later. Use a window and application interface
#include "Windows/Window.h"
#include "Windows/WindowApplication.h"

namespace dsr
{
	class EventDispatcher
	{
	public:
		EventDispatcher(
			const std::shared_ptr<dsr::windows::Window>& window,
			const std::shared_ptr<dsr::windows::WindowApplication>& windowApplication
		);
		EventDispatcher(const EventDispatcher& other) = delete;
		EventDispatcher& operator=(const EventDispatcher& other) = delete;

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::KeyDownEvent&))
		{
			m_window->GetKeyDownEventRegister().Hook(instance, func);
		}

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::KeyUpEvent&))
		{
			m_window->GetKeyUpEventRegister().Hook(instance, func);
		}

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::MouseDownEvent&))
		{
			m_window->GetMouseDownEventRegister().Hook(instance, func);
		}

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::MouseUpEvent&))
		{
			m_window->GetMouseUpEventRegister().Hook(instance, func);
		}

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::MouseMoveEvent&))
		{
			m_window->GetMouseMoveEventRegister().Hook(instance, func);
		}

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::LooseFocusEvent&))
		{
			m_window->GetLooseFocusEventRegister().Hook(instance, func);
		}

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::PrepareUdateFrameEvent&))
		{
			m_windowApplication->GetPrepareUpdateFrameEventRegister().Hook(instance, func);
		}

		template<class TSubType>
		void RegisterEventListener(
			const std::shared_ptr<dsr::events::EventListener>& instance,
			void(TSubType::* func)(const dsr::events::UpdateFrameEvent&))
		{
			m_windowApplication->GetUpdateFrameEventRegister().Hook(instance, func);
		}
	private:
		std::shared_ptr<dsr::windows::Window> m_window;
		std::shared_ptr<dsr::windows::WindowApplication> m_windowApplication;
	};
}
