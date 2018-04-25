#pragma once
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "RenderingSettings.h"

class GraphicsAPI
{
public:
	GraphicsAPI();
	~GraphicsAPI();
	ID3D11Device* getDxDevice();
	ID3D11DeviceContext* getDxDeviceContext();
	bool initialize(struct RenderingSettings rendering_settings);
	void destroy();
	void begin(float red, float green, float blue, float alpha);
	void end();

	void getProjectionMatrix(DirectX::XMMATRIX& projection_matrix);
	void getWorldMatrix(DirectX::XMMATRIX& world_matrix);

private:

	bool initializeVideoCardInfo(float screen_height, float screen_width);
	unsigned int num_modes, m_numerator, m_denominator;
	int m_video_card_memory;
	char m_video_card_description[128];
	bool vsync;
	IDXGISwapChain * m_swap_chain;
	ID3D11Device* m_dx_device;
	ID3D11DeviceContext* m_dx_device_context;
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11Texture2D* m_depth_stencil_buffer;
	ID3D11DepthStencilState* m_depth_stencil_state;
	ID3D11DepthStencilView* m_depth_stencil_view;
	ID3D11RasterizerState* m_rasterizer_state;
	DirectX::XMMATRIX m_projection_matrix;
	DirectX::XMMATRIX m_world_matrix;
	DirectX::XMMATRIX m_orthographic_matrix;
};

