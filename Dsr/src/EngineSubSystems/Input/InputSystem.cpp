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

		void InputSystem::RegisterEvents(std::shared_ptr<dsr::EventDispatcher> dispatcher)
		{
			using namespace dsr::inputdevices;

			dispatcher->RegisterEventListener(m_keyboard, &Keyboard::OnKeyDown);
			dispatcher->RegisterEventListener(m_keyboard, &Keyboard::OnKeyUp);
			dispatcher->RegisterEventListener(m_keyboard, &Keyboard::OnLooseFocus);

			dispatcher->RegisterEventListener(m_mouse, &Mouse::OnMouseDown);
			dispatcher->RegisterEventListener(m_mouse, &Mouse::OnMouseUp);
			dispatcher->RegisterEventListener(m_mouse, &Mouse::OnMouseWheelRotated);
			dispatcher->RegisterEventListener(m_mouse, &Mouse::OnMouseMove);
			dispatcher->RegisterEventListener(m_mouse, &Mouse::OnLooseFocus);
		}
	}
}
