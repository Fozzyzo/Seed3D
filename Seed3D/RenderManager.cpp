#include "RenderManager.h"

RenderManager::RenderManager()
{
	m_dx_api = 0;
}

RenderManager::~RenderManager()
{
}

bool RenderManager::startUp(RenderingSettings rendering_settings)
{
	m_dx_api = new GraphicsAPI();

	if (!m_dx_api->initialize(rendering_settings))
	{
		MessageBox(rendering_settings.window_handle, (LPCSTR)"DirectX could not be initialized", (LPCSTR)"ERROR", MB_OK);
		return false;
	}

	return true;
}

void RenderManager::shutDown()
{
	m_dx_api->destroy();
	m_dx_api = 0;
}

void RenderManager::render()
{
	m_dx_api->drawBuffer(0.2f, 0.2f, 0.2f, 1.0f);
	m_dx_api->swapBuffer();

	return;
}
