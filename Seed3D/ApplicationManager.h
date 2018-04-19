#pragma once
#include <Windows.h>

class ApplicationManager
{

public:

	static LRESULT CALLBACK handleApplicationMessages(HWND window_handle, UINT message,
		WPARAM w_message_parameter, LPARAM l_message_parameter);

	ApplicationManager(HINSTANCE instance_handle, LPCSTR application_name);
	~ApplicationManager();
	bool createApplicationWindow(LPCSTR window_text, int size_x, int size_y);
	WNDCLASSEX getWindowClass();
	HWND getWindowHandle();
	void Shutdown();

	
	bool quit;

private:

	HINSTANCE m_instance_handle;
	HWND m_window_handle;
	WNDCLASSEX m_window_class;
	LPCSTR m_application_name;
};

