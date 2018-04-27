#include "ApplicationManager.h"
#include "InputManager.h"

int WINAPI WinMain(HINSTANCE instance_handle, HINSTANCE previous_instance_handle, PSTR command_line, int cmd_show)
{
	ApplicationManager application_manager = ApplicationManager(instance_handle, "Seed3dApplication");
	RenderManager render_manager;
	InputManager input_manager;
	int mouse_x = 0;
	int mouse_y = 0;
	MSG message = {};

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

	KeyboardPresses layout;

	if (render_manager.startUp(settings) == false)
	{
		application_manager.quit = true;
	}

	if (input_manager.initialize(instance_handle, application_manager.getWindowHandle(), 1280, 800) == false)
	{
		application_manager.quit = true;
	}

	while (!application_manager.quit)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (message.message == WM_QUIT)
		{
			application_manager.quit = true;
		}

		input_manager.handle();

		if (input_manager.isEscapePressed() == true)
		{
			application_manager.quit = true;
		}

		input_manager.getMouseLocation(mouse_x, mouse_y);
		input_manager.getKeyPresses(layout);
		render_manager.handleInput(mouse_x, mouse_y, layout);
		render_manager.render();
	}

	input_manager.shutdown();
	render_manager.shutDown();
	application_manager.shutdown();
	return 0;
}