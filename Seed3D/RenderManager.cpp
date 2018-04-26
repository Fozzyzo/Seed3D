#include "RenderManager.h"

RenderManager::RenderManager()
{
	m_dx_api = 0;
	m_camera = 0;
	m_mesh = 0;
	m_shader_manager = 0;
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

	if (!m_mesh->initialize(m_dx_api->getDxDevice(), m_dx_api->getDxDeviceContext(), (char*)"textures/metal_plate_color.tga", (char*)"meshes/sphere_lowpoly.obj"))
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
	if (!m_shader_manager)
	{
		m_shader_manager->shutdown();
		delete m_shader_manager;
		m_shader_manager = 0;
	}

	if (!m_mesh)
	{
		m_mesh->destroy();
		delete m_mesh;
		m_mesh = 0;
	}

	if (!m_camera)
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

	m_shader_manager->renderShader(m_dx_api->getDxDeviceContext(), m_mesh->getIndexCount(), world_matrix, view_matrix, projection_matrix, m_mesh->getTexture());
	
	m_dx_api->end();


	return;
}

void RenderManager::handleInput(int mouse_x, int mouse_y)
{
	//m_camera->setRotation((800 - mouse_x * 0.10), (1280 - mouse_y * 0.10), 0);
	return;
}
