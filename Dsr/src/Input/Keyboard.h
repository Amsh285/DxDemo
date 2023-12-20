#pragma once

namespace dsr
{
	namespace input
	{
		enum class KeyState
		{
			None,
			Down,
			DownRepeat,
			Up
		};

		class Keyboard
		{
		public:
			Keyboard();

			bool QueryState(const uint8_t& keyCode, const KeyState& state);
		private:
			std::unordered_map<uint8_t, KeyState> m_stateMap;
		};
	}
}
