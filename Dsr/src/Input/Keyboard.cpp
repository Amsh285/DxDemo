#include "dsrpch.h"
#include "Keyboard.h"

dsr::input::Keyboard::Keyboard()
{
}

void dsr::input::Keyboard::OnKeyDown(const dsr::events::KeyDownEvent& keyDown)
{
	m_currentState[keyDown.GetKeyCode()] = true;
}

void dsr::input::Keyboard::OnKeyUp(const dsr::events::KeyUpEvent& keyUp)
{
	m_currentState[keyUp.GetKeyCode()] = false;
}

void dsr::input::Keyboard::OnPrepareUpdateFrame(const dsr::events::PrepareUdateFrameEvent& prepareUpdate)
{
	m_previousState = m_currentState;
}

bool dsr::input::Keyboard::IsKeyDown(const uint8_t& keyCode)
{
	return m_currentState[keyCode];
}

bool dsr::input::Keyboard::IsKeyHolding(const uint8_t& keyCode)
{
	return m_currentState[keyCode] && m_previousState[keyCode];
}

bool dsr::input::Keyboard::IsKeyUp(const uint8_t& keyCode)
{
	return !m_currentState[keyCode] && m_previousState[keyCode];
}
