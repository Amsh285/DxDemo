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

			bool Equals(const MousePosition& rhs) const { return X == rhs.X && Y == rhs.Y; }

			MousePosition()
				: X(0), Y(0)
			{
			}

			MousePosition(const int32_t& x, const int32_t& y)
				: X(x), Y(y)
			{
			}
		};

		class MouseEvent : public IEvent
		{
		public:
			MousePosition GetPosition() const { return m_position; }

			MouseEvent(const int32_t& x, const int32_t& y) : m_position(MousePosition(x, y)) {}
			MouseEvent(const MousePosition& position) : m_position(position) {}
		private:
			MousePosition m_position;
		};

		class MouseKeyEvent : public MouseEvent
		{
		public:
			uint16_t GetKeyCode() const { return m_keyCode; }

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

		class MouseHoverEvent : public MouseEvent
		{
		public:
			MouseHoverEvent(const int32_t& x, const int32_t& y)
				: MouseEvent(x, y)
			{
			}

			MouseHoverEvent(const MousePosition& position)
				: MouseEvent(position)
			{
			}
		private:
		};

		class MouseLeaveEvent
		{
		public:
		private:
		};

		class MouseWheelEvent : public MouseEvent
		{
		public:
			int16_t GetDeltaZ() const { return m_deltaZ; }

			MouseWheelEvent(const int32_t& x, const int32_t& y, const int16_t& deltaZ)
				: MouseEvent(x, y), m_deltaZ(deltaZ)
			{
			}

			MouseWheelEvent(const MousePosition& position, const int16_t& deltaZ)
				: MouseEvent(position), m_deltaZ(deltaZ)
			{
			}
		private:
			int16_t m_deltaZ;
		};
	}
}
