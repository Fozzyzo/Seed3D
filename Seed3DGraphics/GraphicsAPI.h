#pragma once
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

namespace S3DGraphics
{
	class GraphicsAPI
	{
	public:
		GraphicsAPI();
		~GraphicsAPI();
		bool initialize();

	private:
		ID3D11Device* m_dx_device;
		ID3D11DeviceContext* m_dx_device_context;
		ID3D11RenderTargetView* m_render_target_view;
		ID3D11Texture2D* m_depth_stencil_buffer;
		ID3D11DepthStencilState* m_depth_stencil_state;
		ID3D11DepthStencilView* m_depth_stencil_view;
		ID3D11RasterizerState* m_rasterizer_state;
		DirectX::XMFLOAT4X4 m_projection_matrix;
		DirectX::XMFLOAT4X4 m_world_matrix;
		DirectX::XMFLOAT4X4 m_orthographic_matrix;
	};
}

