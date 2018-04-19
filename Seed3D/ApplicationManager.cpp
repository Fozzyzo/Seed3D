#include "ApplicationManager.h"
LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_message_parameter, LPARAM l_message_parameter);

ApplicationManager::ApplicationManager(HINSTANCE instance_handle, LPCSTR application_name)
{
	m_window_handle = {};
	m_instance_handle = instance_handle;
	m_window_class = {};

	m_window_class.cbClsExtra = 0;
	m_window_class.cbWndExtra = 0;
	m_window_class.cbSize = sizeof(WNDCLASSEX);
	m_window_class.lpfnWndProc = WndProc;
	m_window_class.lpszClassName = application_name;
	m_window_class.hInstance = instance_handle;
	m_window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;;
	m_window_class.hCursor = LoadCursor(0, IDC_ARROW);
	m_window_class.hIcon = LoadIcon(0, IDI_WINLOGO);
	m_window_class.hIconSm = m_window_class.hIcon;
	m_window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClassEx(&m_window_class);
}

ApplicationManager::~ApplicationManager()
{
}

bool ApplicationManager::createApplicationWindow(LPCSTR window_text, int width, int height)
{
	m_window_handle = CreateWindowEx(WS_EX_APPWINDOW,
		m_window_class.lpszClassName,
		(LPCSTR) window_text,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, m_instance_handle, NULL);

	if (m_window_handle == NULL)
	{
		DWORD error = GetLastError();
		return false;
	}

	else
	{
		return true;
	}
}

LRESULT CALLBACK ApplicationManager::handleApplicationMessages(HWND window_handle, UINT message, WPARAM w_message_parameter, LPARAM l_message_parameter)
{
	switch (message)
	{
		case WM_KEYDOWN:
			return 0;

		case WM_KEYUP:
			return 0;

		default:
		{
			return DefWindowProc(window_handle, message, w_message_parameter, l_message_parameter);
		}
	}
}

WNDCLASSEX ApplicationManager::getWindowClass()
{
	return m_window_class;
}

HWND ApplicationManager::getWindowHandle()
{
	return m_window_handle;
}

void ApplicationManager::Shutdown()
{
	DestroyWindow(m_window_handle);
	m_window_handle = NULL;

	UnregisterClass(m_window_class.lpszClassName, m_instance_handle);
	m_instance_handle = NULL;
}

LRESULT CALLBACK WndProc(HWND window_handle, UINT message,
	WPARAM w_message_parameter, LPARAM l_message_parameter)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return ApplicationManager::handleApplicationMessages(window_handle, message,
				w_message_parameter, l_message_parameter);
		}
	}
}
