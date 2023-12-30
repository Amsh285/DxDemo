#pragma once

#include "SubSystems/Input/KeyCode.h"

namespace dsr
{
	namespace input
	{
		class KeyMap
		{
		public:
			

			virtual void Setup() = 0;
		protected:
			std::unordered_map<KeyCode, uint16_t> m_keyCodeMap;
		private:
		};
	}
}
