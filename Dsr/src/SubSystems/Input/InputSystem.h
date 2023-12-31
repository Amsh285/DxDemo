#pragma once

#include "Events/Application/InputEvents/KeyboardEvents.h"
#include "Events/Application/InputEvents/MouseEvents.h"
#include "Events/Application/WindowEvents.h"
#include "Events/EventRegister.h"
#include "SubSystems/Input/Input.h"

namespace dsr
{
	namespace input
	{
		class InputSystem
		{
		public:
			std::shared_ptr<dsr::inputdevices::Keyboard> GetKeyboard() const { return m_keyboard; }
			std::shared_ptr<dsr::inputdevices::Mouse> GetMouse() const { return m_mouse; }
			std::shared_ptr<Input> GetInput() const { return m_input; }

			InputSystem(const std::unordered_map<KeyCode, uint16_t>& keyMap);
			InputSystem(const InputSystem& other) = delete;
			InputSystem& operator=(const InputSystem& other) = delete;

			void RegisterEvents(
				EventRegisterType<const dsr::events::KeyDownEvent&>& keyDownEventRegister,
				EventRegisterType<const dsr::events::KeyUpEvent&>& keyUpEventRegister,
				EventRegisterType<const dsr::events::MouseDownEvent&>& mouseDownEventRegister,
				EventRegisterType<const dsr::events::MouseUpEvent&>& mouseUpEventRegister,
				EventRegisterType<const dsr::events::MouseMoveEvent&>& mouseMoveEventRegister,
				EventRegisterType<const dsr::events::PrepareUdateFrameEvent&>& prepareUpdateFrameEventRegister
			);
		private:
			std::shared_ptr<dsr::inputdevices::Keyboard> m_keyboard;
			std::shared_ptr<dsr::inputdevices::Mouse> m_mouse;
			std::shared_ptr<Input> m_input;
		};
	}
}
