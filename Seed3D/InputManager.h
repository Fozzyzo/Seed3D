#pragma once
#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputManager
{
public:
	InputManager();
	~InputManager();
	bool initialize(HINSTANCE instance, HWND window_handle, int width, int height);
	void shutdown();
	bool handle();

	bool isEscapePressed();
	void getMouseLocation(int& mouse_x, int& mouse_y);

private:
	bool readKeyboard();
	bool readMouse();
	void processInput();

	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboard_state[256];
	DIMOUSESTATE m_mouse_state;

	int m_screen_width;
	int m_screen_height;
	int m_mouse_x;
	int m_mouse_y;
};

