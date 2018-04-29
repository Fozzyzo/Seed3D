#include "RenderManager.h"

RenderManager::RenderManager()
{
	m_dx_api = 0;
	m_camera = 0;
	m_mesh = 0;
	m_shader_manager = 0;
	m_light = 0;
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

	m_camera = new Camera();
	m_camera->setPosition(0.0f, 0.0f, -10.0f);

	m_mesh = new Mesh();

	m_light = new DirectionalLight();
	m_light->setAmbientLight(0.30f, 0.30f, 0.30f, 1.0f);
	m_light->setDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	m_light->setDirection(0.0f, 0.0f, 1.0f);
	m_light->setSpecularColor(0.8f, 0.8f, 0.8f, 1.0f);
	m_light->setSpecularPower(16.0f);

	if (!m_mesh->initialize(m_dx_api->getDxDevice(), m_dx_api->getDxDeviceContext(), (char*)"meshes/sphere.obj", (char*)"textures/metal_plate2_diffuse.tga", (char*)"textures/metal_plate2_normal.tga", 0.0f))
	{
		MessageBox(rendering_settings.window_handle, "Object initialization failed", "Error", MB_OK);
		return false;
	}

	m_shader_manager = new ShaderManager();

	if (!m_shader_manager->initialize(m_dx_api->getDxDevice(), rendering_settings.window_handle))
	{
		MessageBox(rendering_settings.window_handle, "Shader initialization failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void RenderManager::shutDown()
{
	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	if (!m_shader_manager)
	{
		m_shader_manager->shutdown();
		delete m_shader_manager;
		m_shader_manager = 0;
	}

	if (m_mesh)
	{
		m_mesh->destroy();
		delete m_mesh;
		m_mesh = 0;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	if (m_dx_api)
	{
		m_dx_api->destroy();
		delete m_dx_api;
		m_dx_api = 0;
	}

	return;
}

void RenderManager::render()
{
	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;
	DirectX::XMMATRIX world_matrix;

	m_dx_api->begin(0.2f, 0.2f, 0.2f, 1.0f);
	
	m_camera->render();

	m_camera->getViewMatrix(view_matrix);
	m_dx_api->getWorldMatrix(world_matrix);
	m_dx_api->getProjectionMatrix(projection_matrix);

	m_mesh->render(m_dx_api->getDxDeviceContext());

	m_shader_manager->renderShader(m_dx_api->getDxDeviceContext(), m_mesh->getIndexCount(), world_matrix, view_matrix, projection_matrix, 
		m_mesh->getMaterial(), m_light->getDirection(), m_light->getAmbientLight(), m_light->getDiffuseColor(), m_camera->getPosition(),
		m_light->getSpecularColor(), m_light->getSpecularStrength());
	
	m_dx_api->end();

	return;
}

void RenderManager::handleInput(int mouse_x, int mouse_y, KeyboardPresses layout)
{
	float velocity_x = 0;
	float velocity_y = 0;
	float velocity_z = 0;

	float rotation_x = 0;
	float rotation_y = 0;
	float rotation_z = 0;

	if (layout.a == true)
	{	
		velocity_x = -0.1f;
	}

	if (layout.d == true)
	{
		velocity_x = 0.1f;
	}

	if (layout.w == true)
	{
		velocity_z = 0.1f;
	}

	if (layout.s == true)
	{
		velocity_z = -0.1f;
	}

	if (layout.space == true)
	{
		velocity_y = 0.1f;
	}

	if (layout.left_shift == true)
	{
		velocity_y = -0.1f;
	}

	if (layout.arrow_left == true)
	{
		rotation_y = -1.0f;
	}

	if (layout.arrow_right == true)
	{
		rotation_y = 1.0f;
	}

	if(layout.arrow_up == true)
	{
		rotation_x = -0.8f;
	}

	if (layout.arrow_down == true)
	{
		rotation_x = 0.8f;
	}


	m_camera->setVelocity(velocity_x, velocity_y, velocity_z);
	m_camera->addRotation(rotation_x, rotation_y, 0);
	return;
}
