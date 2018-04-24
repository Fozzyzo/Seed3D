#include "GraphicsAPI.h"

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

ID3D11Device* GraphicsAPI::getDxDevice()
{
	return m_dx_device;
}

ID3D11DeviceContext* GraphicsAPI::getDxDeviceContext()
{
	return m_dx_device_context;
}

bool GraphicsAPI::initialize(RenderingSettings rendering_settings)
{
	int error;
	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	D3D_FEATURE_LEVEL feature_level;
	ID3D11Texture2D* backbuffer_ptr;
	D3D11_TEXTURE2D_DESC depth_buffer_description;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_description;
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_description;
	D3D11_RASTERIZER_DESC raster_description;
	D3D11_VIEWPORT viewport;
	float field_of_view, screen_aspect;

	initializeVideoCardInfo(rendering_settings.screen_height, rendering_settings.screen_width);
	
	vsync = rendering_settings.vsync;
	//	Swap chain inizialization
	ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));

	swap_chain_description.BufferCount = 1;
	swap_chain_description.BufferDesc.Width = rendering_settings.screen_width;
	swap_chain_description.BufferDesc.Height = rendering_settings.screen_height;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow = rendering_settings.window_handle;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_description.Flags = 0;
	swap_chain_description.Windowed = true;

	if (rendering_settings.vsync)
	{
		swap_chain_description.BufferDesc.RefreshRate.Numerator = m_numerator;
		swap_chain_description.BufferDesc.RefreshRate.Denominator = m_denominator;
	}

	else
	{
		swap_chain_description.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	}

	feature_level = D3D_FEATURE_LEVEL_11_0;

	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &feature_level, 1,
				D3D11_SDK_VERSION, &swap_chain_description, &m_swap_chain, &m_dx_device, NULL, &m_dx_device_context)))
	{ return false; }

	if(FAILED(m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer_ptr)))
	{ return false; }

	if(FAILED(m_dx_device->CreateRenderTargetView(backbuffer_ptr, NULL, &m_render_target_view)))
	{ return false; }

	backbuffer_ptr->Release();
	backbuffer_ptr = 0;

	//	Depth Buffer inialization
	
	ZeroMemory(&depth_buffer_description, sizeof(depth_buffer_description));

	depth_buffer_description.Width = rendering_settings.screen_width;
	depth_buffer_description.Height = rendering_settings.screen_height;
	depth_buffer_description.MipLevels = 1;
	depth_buffer_description.ArraySize = 1;
	depth_buffer_description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_description.SampleDesc.Count = 1;
	depth_buffer_description.SampleDesc.Quality = 0;
	depth_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_buffer_description.CPUAccessFlags = 0;
	depth_buffer_description.MiscFlags = 0;

	if (FAILED(m_dx_device->CreateTexture2D(&depth_buffer_description, NULL, &m_depth_stencil_buffer)))
		return false;

	//	Stencil state initialization

	ZeroMemory(&depth_stencil_description, sizeof(depth_stencil_description));

	depth_stencil_description.DepthEnable = true;
	depth_stencil_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_description.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_description.StencilEnable = true;
	depth_stencil_description.StencilReadMask = 0xFF;
	depth_stencil_description.StencilWriteMask = 0xFF;

	depth_stencil_description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depth_stencil_description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	if (FAILED(m_dx_device->CreateDepthStencilState(&depth_stencil_description, &m_depth_stencil_state)))
		return false;

	m_dx_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);

	// Depth stencil buffer initialization

	ZeroMemory(&depth_stencil_view_description, sizeof(depth_stencil_view_description));

	depth_stencil_view_description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_description.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_description.Texture2D.MipSlice = 0;

	if (FAILED(m_dx_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_description, &m_depth_stencil_view)))
		return false;

	m_dx_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

	//	Rasterizer initialization
	ZeroMemory(&raster_description, sizeof(raster_description));

	raster_description.AntialiasedLineEnable = false;
	raster_description.CullMode = D3D11_CULL_BACK;
	raster_description.DepthBias = 0;
	raster_description.DepthBiasClamp = 0.0f;
	raster_description.DepthClipEnable = true;
	raster_description.FillMode = D3D11_FILL_SOLID;
	raster_description.FrontCounterClockwise = false;
	raster_description.MultisampleEnable = false;
	raster_description.ScissorEnable = false;
	raster_description.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_dx_device->CreateRasterizerState(&raster_description, &m_rasterizer_state)))
		return false;

	m_dx_device_context->RSSetState(m_rasterizer_state);

	viewport.Width = rendering_settings.screen_width;
	viewport.Height = rendering_settings.screen_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_dx_device_context->RSSetViewports(1, &viewport);

	field_of_view = (float)DirectX::XM_PI / 4.0f;
	screen_aspect = (float)rendering_settings.screen_width / (float)rendering_settings.screen_height;

	//Matrix initialization
	m_projection_matrix = DirectX::XMMatrixPerspectiveFovLH(field_of_view, screen_aspect, rendering_settings.screen_near, rendering_settings.screen_depth);
	m_world_matrix = DirectX::XMMatrixIdentity();
	m_orthographic_matrix = DirectX::XMMatrixOrthographicLH(rendering_settings.screen_width, rendering_settings.screen_height, rendering_settings.screen_near, rendering_settings.screen_depth);

	return true;
}

bool GraphicsAPI::initializeVideoCardInfo(float screen_height, float screen_width)
{
	HRESULT result;
	IDXGIFactory* dx_factory;
	IDXGIAdapter* dx_adapter;
	IDXGIOutput* dx_adapter_output;
	DXGI_MODE_DESC* dx_mode_description;
	DXGI_ADAPTER_DESC dx_adapter_description;
	int error;


	if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dx_factory)))
	{ return false;  }

	if (FAILED(dx_factory->EnumAdapters(0, &dx_adapter)))
	{ return false; }

	if (FAILED(dx_adapter->EnumOutputs(0, &dx_adapter_output)))
	{ return false; }

	if (FAILED(dx_adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL)))
	{ return false; }

	dx_mode_description = new DXGI_MODE_DESC[num_modes];
	if (!dx_mode_description)
	{ return false; }

	if(FAILED(dx_adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, dx_mode_description)))
	{ return false; }

	for (int i = 0; i < num_modes; i++)
	{
		if (dx_mode_description[i].Width == (unsigned int)screen_width)
		{
			if (dx_mode_description[i].Height == (unsigned int)screen_height)
			{
				m_numerator = dx_mode_description[i].RefreshRate.Numerator;
				m_denominator = dx_mode_description[i].RefreshRate.Denominator;
			}
		}
	}

	if(FAILED(dx_adapter->GetDesc(&dx_adapter_description)))
	{ return false; }
	
	size_t string_length;
	m_video_card_memory = (int)(dx_adapter_description.DedicatedVideoMemory / 1024 / 1024);
	error = wcstombs_s(&string_length, m_video_card_description, 128, dx_adapter_description.Description, 128);
	if (error != 0)
	{
		return false;
	}

	delete[] dx_mode_description;
	dx_mode_description = 0;

	dx_adapter_output->Release();
	dx_adapter_output = 0;

	dx_adapter->Release();
	dx_adapter = 0;

	dx_factory->Release();
	dx_factory = 0;

	return true;
}

void GraphicsAPI::destroy()
{
	if (m_rasterizer_state)
	{
		m_rasterizer_state->Release();
		m_rasterizer_state = 0;
	}

	if (m_depth_stencil_view)
	{
		m_depth_stencil_view->Release();
		m_depth_stencil_view = 0;
	}

	if (m_depth_stencil_state)
	{
		m_depth_stencil_state->Release();
		m_depth_stencil_state = 0;
	}

	if (m_depth_stencil_buffer)
	{
		m_depth_stencil_buffer->Release();
		m_depth_stencil_buffer = 0;
	}

	if (m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = 0;
	}

	if (m_dx_device_context)
	{
		m_dx_device_context->Release();
		m_dx_device_context = 0;
	}

	if (m_dx_device)
	{
		m_dx_device->Release();
		m_dx_device = 0;
	}

	if (m_swap_chain)
	{
		m_swap_chain->Release();
		m_swap_chain = 0;
	}

	return;
}

void GraphicsAPI::begin(float red, float green, float blue, float alpha)
{
	float colors[4];

	colors[0] = red;
	colors[1] = green;
	colors[2] = blue;
	colors[3] = alpha;

	m_dx_device_context->ClearRenderTargetView(m_render_target_view, colors);

	m_dx_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void GraphicsAPI::end()
{
	if (vsync)
	{
		m_swap_chain->Present(1, 0);
	}

	else
	{
		m_swap_chain->Present(0, 0);
	}
}

void GraphicsAPI::getProjectionMatrix(DirectX::XMMATRIX& world_matrix)
{
	world_matrix = m_world_matrix;
	return;
}

void GraphicsAPI::getWorldMatrix(DirectX::XMMATRIX& projection_matrix)
{
	projection_matrix = m_projection_matrix;
	return;
}

