#pragma once
#include <Windows.h>
#include "GraphicsAPI.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include "DirectionalLight.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	bool startUp(struct RenderingSettings rendering_settings);
	void shutDown();
	void render();
	void handleInput(int mouse_x, int mouse_y, struct KeyboardPresses layout);

private:
	GraphicsAPI* m_dx_api;
	Camera* m_camera;
	Mesh* m_mesh;
	ShaderManager* m_shader_manager;
	DirectionalLight* m_light;
};
