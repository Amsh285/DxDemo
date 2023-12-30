#pragma once

#include "InputDevices/Keyboard.h"
#include "InputDevices/Mouse.h"

namespace dsr
{
	namespace subsystems
	{
		class Input
		{
		public:
			Input();
		protected:
			std::shared_ptr<dsr::inputdevices::Keyboard> m_keyboard;
			std::shared_ptr<dsr::inputdevices::Mouse> m_mouse;
		};
	}
}
