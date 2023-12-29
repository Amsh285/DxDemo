#pragma once

#include "Events/IEvent.h"

namespace dsr
{
	namespace events
	{
		struct MousePosition
		{
			int32_t X;
			int32_t Y;

			MousePosition(const int32_t& x, const int32_t& y)
				: X(x), Y(y)
			{
			}
		};

		class MouseEvent : public IEvent
		{
		public:
			const MousePosition& GetPosition() const { return m_position; }

			MouseEvent(const int32_t& x, const int32_t& y) : m_position(MousePosition(x, y)) {}
			MouseEvent(const MousePosition& position) : m_position(position) {}
		private:
			MousePosition m_position;
		};

		class MouseKeyEvent : public MouseEvent
		{
		public:
			const uint16_t& GetKeyCode() const { return m_keyCode; }

			MouseKeyEvent(const int32_t& x, const int32_t& y, const uint16_t& keyCode)
				: MouseEvent(x, y), m_keyCode(keyCode)
			{
			}

			MouseKeyEvent(const MousePosition& position, const uint16_t& keyCode)
				: MouseEvent(position), m_keyCode(keyCode)
			{
			}
		private:
			uint16_t m_keyCode;
		};

		class MouseDownEvent : public MouseKeyEvent
		{ 
		public:
			MouseDownEvent(const int32_t& x, const int32_t& y, const uint16_t& keyCode)
				: MouseKeyEvent(x, y, keyCode)
			{
			}

			MouseDownEvent(const MousePosition& position, const uint16_t& keyCode)
				: MouseKeyEvent(position, keyCode)
			{
			}
		private:
		};

		class MouseUpEvent : public MouseKeyEvent
		{
		public:
			MouseUpEvent(const int32_t& x, const int32_t& y, const uint16_t& keyCode)
				: MouseKeyEvent(x, y, keyCode)
			{
			}

			MouseUpEvent(const MousePosition& position, const uint16_t& keyCode)
				: MouseKeyEvent(position, keyCode)
			{
			}
		private:
		};

		class MouseMoveEvent : public MouseEvent
		{
		public:
			MouseMoveEvent(const int32_t& x, const int32_t& y)
				: MouseEvent(x, y)
			{
			}

			MouseMoveEvent(const MousePosition& position)
				: MouseEvent(position)
			{
			}
		private:
		};
	}
}
