#include "dsrpch.h"
#include "InputSystem.h"

namespace dsr
{
	namespace input
	{
		InputSystem::InputSystem(const std::unordered_map<KeyCode, uint16_t>& keyMap)
		{
			m_keyboard = std::make_shared<dsr::inputdevices::Keyboard>();
			m_mouse = std::make_shared<dsr::inputdevices::Mouse>();
			m_input = std::make_shared<Input>(m_keyboard, m_mouse, keyMap);
		}

		void InputSystem::RegisterEvents(
			EventRegisterType<const dsr::events::KeyDownEvent&>& keyDownEventRegister,
			EventRegisterType<const dsr::events::KeyUpEvent&>& keyUpEventRegister,
			EventRegisterType<const dsr::events::MouseDownEvent&>& mouseDownEventRegister,
			EventRegisterType<const dsr::events::MouseUpEvent&>& mouseUpEventRegister,
			EventRegisterType<const dsr::events::MouseMoveEvent&>& mouseMoveEventRegister,
			EventRegisterType<const dsr::events::PrepareUdateFrameEvent&>& prepareUpdateFrameEventRegister)
		{
			using namespace dsr::inputdevices;
			
			keyDownEventRegister.Hook(m_keyboard, &Keyboard::OnKeyDown);
			keyUpEventRegister.Hook(m_keyboard, &Keyboard::OnKeyUp);

			mouseDownEventRegister.Hook(m_mouse, &Mouse::OnMouseDown);
			mouseUpEventRegister.Hook(m_mouse, &Mouse::OnMouseUp);
			mouseMoveEventRegister.Hook(m_mouse, &Mouse::OnMouseMove);

			prepareUpdateFrameEventRegister.Hook(m_keyboard, &Keyboard::OnPrepareUpdateFrame);
			prepareUpdateFrameEventRegister.Hook(m_mouse, &Mouse::OnPrepareUpdateFrame);
		}
	}
}
