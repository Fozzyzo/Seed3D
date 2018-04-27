#include "InputManager.h"


InputManager::InputManager()
{
	m_direct_input = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputManager::~InputManager()
{
}

bool InputManager::initialize(HINSTANCE instance, HWND window_handle, int width, int height)
{
	m_screen_height = height;
	m_screen_width = width;

	m_mouse_x = 0;
	m_mouse_y = 0;

	if (FAILED(DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL)))
	{
		return false;
	}

	if (FAILED(m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL)))
	{
		return false;
	}

	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	if (FAILED(m_keyboard->Acquire()))
	{
		return false;
	}

	if (FAILED(m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse, NULL)))
	{
		return false;
	}

	if (FAILED(m_mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}

	if (FAILED(m_mouse->SetCooperativeLevel(window_handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}
	/*
	if (FAILED(m_mouse->Acquire()))
	{
		return false;
	}
	*/
	return true;
}

void InputManager::shutdown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_direct_input)
	{
		m_direct_input->Release();
		m_direct_input = 0;
	}

	return;
}

bool InputManager::handle()
{
	if (readKeyboard())
	{
		return false;
	}

	if (readMouse())
	{
		return false;
	}

	processInput();

	return true;
}

bool InputManager::isEscapePressed()
{
	if (m_keyboard_state[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void InputManager::getKeyPresses(KeyboardPresses& keys)
{
	keys.a = false;
	keys.w = false;
	keys.d = false;
	keys.s = false;
	keys.arrow_up = false;
	keys.arrow_left = false;
	keys.arrow_down = false;
	keys.arrow_right = false;

	if (m_keyboard_state[DIK_W] & 0x80)
	{
		keys.w = true;
	}

	if (m_keyboard_state[DIK_A] & 0x80)
	{
		keys.a = true;
	}

	if (m_keyboard_state[DIK_S] & 0x80)
	{
		keys.s = true;
	}

	if (m_keyboard_state[DIK_D] & 0x80)
	{
		keys.d = true;
	}

	if (m_keyboard_state[DIK_UPARROW] & 0x80)
	{
		keys.arrow_up = true;
	}

	if (m_keyboard_state[DIK_LEFTARROW] & 0x80)
	{
		keys.arrow_left = true;
	}

	if (m_keyboard_state[DIK_RIGHTARROW] & 0x80)
	{
		keys.arrow_right = true;
	}

	if (m_keyboard_state[DIK_DOWNARROW] & 0x80)
	{
		keys.arrow_down = true;
	}

	return;
}

void InputManager::getMouseLocation(int& mouse_x, int& mouse_y)
{
	mouse_x = m_mouse_x;
	mouse_y = m_mouse_y;
}

bool InputManager::readKeyboard()
{
	if (FAILED(m_keyboard->GetDeviceState(sizeof(m_keyboard_state), (LPVOID)&m_keyboard_state)))
	{
		return false;
	}

	return true;
}

bool InputManager::readMouse()
{
	m_mouse->Acquire();

	if (FAILED(m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouse_state)))
	{
		return false;
	}

	return true;
}

void InputManager::processInput()
{
	m_mouse_x += m_mouse_state.lX;
	m_mouse_x += m_mouse_state.lY;

	if (m_mouse_x < 0)
	{
		m_mouse_x = 0;
	}

	if (m_mouse_y < 0)
	{
		m_mouse_y = 0;
	}

	if (m_mouse_x > m_screen_width)
	{
		m_mouse_x = m_screen_width;
	}

	if (m_mouse_y > m_screen_height)
	{
		m_mouse_y = m_screen_height;
	}

	return;
}
