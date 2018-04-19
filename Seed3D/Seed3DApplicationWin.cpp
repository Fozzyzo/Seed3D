#include "ApplicationManager.h"


int WINAPI WinMain(HINSTANCE instance_handle, HINSTANCE previous_instance_handle, PSTR command_line, int cmd_show)
{
	ApplicationManager application_manager = ApplicationManager(instance_handle, "Seed3dApplication");
	MSG message = {};
	bool quit = false;

	if (!application_manager.createApplicationWindow("3D application demo", 1280, 800))
	{
		return 1;
	}

	ShowWindow(application_manager.getWindowHandle(), cmd_show);

	while (!application_manager.quit)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE));
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (message.message == WM_QUIT)
		{
			quit = true;
		}
	}

	application_manager.Shutdown();
	return 0;
}