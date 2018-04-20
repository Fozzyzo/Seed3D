#include "GraphicsAPI.h"
#include "stdafx.h"

namespace S3DGraphics
{
	GraphicsAPI::GraphicsAPI()
	{
		m_swap_chain = 0;
		m_dx_device = 0;
		m_dx_device_context = 0;
		m_render_target_view = 0;
		m_depth_stencil_buffer = 0;
		m_depth_stencil_state = 0;
		m_depth_stencil_view = 0;
		m_rasterizer_state = 0;
		m_projection_matrix = {};
		m_world_matrix = {};
		m_orthographic_matrix = {};
	}


	GraphicsAPI::~GraphicsAPI()
	{
	}

	bool GraphicsAPI::initialize()
	{
		return true;
	}
}
