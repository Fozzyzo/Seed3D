#pragma once
#include <Windows.h>

struct RenderingSettings
{
	HWND window_handle;
	float screen_width;
	float screen_height;
	float screen_depth;
	float screen_near;
	bool vsync;
};
