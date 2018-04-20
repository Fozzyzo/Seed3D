#pragma once
#include <Windows.h>
#include "GraphicsAPI.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	bool startUp(struct RenderingSettings rendering_settings);
	void shutDown();
	void render();

private:
	GraphicsAPI* m_dx_api;
};
