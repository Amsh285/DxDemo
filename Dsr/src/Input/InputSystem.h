#pragma once

#include "Events/EventRegister.h"
#include "Events/Application/WindowEvents.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace dsr
{
	namespace input
	{
		//Todo
		class InputSystem
		{
		public:
			std::shared_ptr<dsr::input::Keyboard> GetKeyboard() const { return m_keyboard; }
			std::shared_ptr<dsr::input::Mouse> GetMouse() const { return m_mouse; }

			InputSystem();
			InputSystem(const InputSystem& other) = delete;
			InputSystem& operator=(const InputSystem& other) = delete;

			void Setup(
				const EventRegisterType<dsr::events::KeyDownEvent>& keyDownEventRegister,
				const EventRegisterType<dsr::events::KeyUpEvent>& keyUpEventRegister,
				const EventRegisterType<dsr::events::MouseDownEvent>& mouseDownEventRegister,
				const EventRegisterType<dsr::events::MouseUpEvent>& mouseUpEventRegister,
				const EventRegisterType<dsr::events::MouseMoveEvent>& mouseMoveEventRegister,
				const EventRegisterType<dsr::events::PrepareUdateFrameEvent>& prepareUpdateFrameEventRegister
			);
		private:
			std::shared_ptr<dsr::input::Keyboard> m_keyboard;
			std::shared_ptr<dsr::input::Mouse> m_mouse;
		};
	}
}
