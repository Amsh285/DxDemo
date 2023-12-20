#include "dsrpch.h"
#include "Keyboard.h"

dsr::input::Keyboard::Keyboard()
{
}

void dsr::input::Keyboard::HandleKeyDown(const dsr::events::KeyDownEvent& keyDown)
{
	m_previousState = m_currentState;
	m_currentState[keyDown.GetKeyCode()] = true;
}

void dsr::input::Keyboard::HandleKeyUp(const dsr::events::KeyUpEvent& keyUp)
{
	m_previousState = m_currentState;
	m_currentState[keyUp.GetKeyCode()] = false;
}

bool dsr::input::Keyboard::IsKeyDown(const uint8_t& keyCode)
{
	return m_currentState[keyCode];
}

bool dsr::input::Keyboard::IsKeyUp(const uint8_t& keyCode)
{
	return !m_currentState[keyCode] && m_previousState[keyCode];
}
