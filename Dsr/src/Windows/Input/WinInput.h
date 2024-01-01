#pragma once

#include "SubSystems/Input/KeyCode.h"

namespace dsr
{
	namespace windows
	{
		std::unordered_map<dsr::input::KeyCode, uint16_t> CreateKeyMap();
	}
}
