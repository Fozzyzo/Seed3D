#include "ApplicationManager.h"

int WINAPI WinMain(HINSTANCE instance_handle, HINSTANCE previous_instance_handle, PSTR command_line, int cmd_show)
{
	ApplicationManager application_manager = ApplicationManager(instance_handle, "Seed3dApplication");
	RenderManager render_manager;

	MSG message = {};
	bool quit = false;

	if (!application_manager.createApplicationWindow("3D application demo", 1280, 800))
	{
		return 1;
	}

	ShowWindow(application_manager.getWindowHandle(), cmd_show);

	RenderingSettings settings;
	settings.window_handle = application_manager.getWindowHandle();
	settings.screen_height = 800;
	settings.screen_width = 1280;
	settings.screen_near = 0.1f;
	settings.screen_depth = 1000.0f;
	settings.vsync = true;
	render_manager.startUp(settings);

	while (!application_manager.quit)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (message.message == WM_QUIT)
		{
			quit = true;
		}

		render_manager.render();
	}
	
	render_manager.shutDown();
	application_manager.shutdown();
	return 0;
}