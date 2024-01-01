#include "dsrpch.h"
#include "WinInput.h"

namespace dsr
{
	namespace windows
	{
		std::unordered_map<dsr::input::KeyCode, uint16_t> CreateKeyMap()
		{
			using namespace dsr::input;

			return {
				{ KeyCode::MouseLeft, MK_LBUTTON },
				{ KeyCode::MouseRight, MK_RBUTTON },
				{ KeyCode::MouseMiddle, MK_MBUTTON },

				// see: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
				{ KeyCode::A, 0x41 },
				{ KeyCode::B, 0x42 },
				{ KeyCode::C, 0x43 },
				{ KeyCode::D, 0x44 },
				{ KeyCode::E, 0x45 },
				{ KeyCode::F, 0x46 },
				{ KeyCode::G, 0x47 },
				{ KeyCode::H, 0x48 },
				{ KeyCode::I, 0x49 },
				{ KeyCode::J, 0x4A },
				{ KeyCode::K, 0x4B },
				{ KeyCode::L, 0x4C },
				{ KeyCode::M, 0x4D },
				{ KeyCode::N, 0x4E },
				{ KeyCode::O, 0x4F },
				{ KeyCode::P, 0x50 },
				{ KeyCode::Q, 0x51 },
				{ KeyCode::R, 0x52 },
				{ KeyCode::S, 0x53 },
				{ KeyCode::T, 0x54 },
				{ KeyCode::U, 0x55 },
				{ KeyCode::V, 0x56 },
				{ KeyCode::W, 0x57 },
				{ KeyCode::X, 0x58 },
				{ KeyCode::Y, 0x59 },
				{ KeyCode::Z, 0x5A },

				{ KeyCode::Alpha0, 0x30 },
				{ KeyCode::Alpha1, 0x31 },
				{ KeyCode::Alpha2, 0x32 },
				{ KeyCode::Alpha3, 0x33 },
				{ KeyCode::Alpha4, 0x34 },
				{ KeyCode::Alpha5, 0x35 },
				{ KeyCode::Alpha6, 0x36 },
				{ KeyCode::Alpha7, 0x37 },
				{ KeyCode::Alpha8, 0x38 },
				{ KeyCode::Alpha9, 0x39 },

				{ KeyCode::F1, 0x70 },
				{ KeyCode::F2, 0x71 },
				{ KeyCode::F3, 0x72 },
				{ KeyCode::F4, 0x73 },
				{ KeyCode::F5, 0x74 },
				{ KeyCode::F6, 0x75 },
				{ KeyCode::F7, 0x76 },
				{ KeyCode::F8, 0x77 },
				{ KeyCode::F9, 0x78 },
				{ KeyCode::F10, 0x79 },
				{ KeyCode::F11, 0x7A },
				{ KeyCode::F12, 0x7B },

				{ KeyCode::Space, VK_SPACE },
				{ KeyCode::Enter, VK_RETURN },
				{ KeyCode::Backspace, VK_BACK },
				{ KeyCode::Escape, VK_ESCAPE },
				
				{ KeyCode::Plus, VK_OEM_PLUS },
				{ KeyCode::Minus, VK_OEM_MINUS },
				{ KeyCode::Comma, VK_OEM_COMMA },
				{ KeyCode::Period, VK_OEM_PERIOD },

				{ KeyCode::ArrowLeft, VK_LEFT },
				{ KeyCode::ArrowUp, VK_UP },
				{ KeyCode::ArrowRight, VK_RIGHT },
				{ KeyCode::ArrowDown, VK_DOWN }
			};

			/* There are multiple misc keys that have different physical locations and keycodes depeendng on the keyboard-layout.
			They will be handled later when i found a good way to do it.
			Until then misc keys have to be handled in client code.*/
			// Hash -> VK_OEM_2
			// On a German keyboard-layout. 
			// On a American layout the hash will be located on the the 3 and the shift button needs to be pressed.
			// Instead VK_OEM_2 will be the / key
		}
	}
}
