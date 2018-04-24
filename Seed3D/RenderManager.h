#pragma once
#include <Windows.h>
#include "GraphicsAPI.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderManager.h"

typedef wchar_t WCHAR;

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
	Camera* m_camera;
	Mesh* m_mesh;
	ShaderManager* m_shader_manager;
};
