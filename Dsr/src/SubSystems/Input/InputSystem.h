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
		class InputSystem : public Input
		{
		public:
			/*std::shared_ptr<dsr::inputdevices::Keyboard> GetKeyboard() const { return m_keyboard; }
			std::shared_ptr<dsr::inputdevices::Mouse> GetMouse() const { return m_mouse; }*/


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

		};
	}
}
