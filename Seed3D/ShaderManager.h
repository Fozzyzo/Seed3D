#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <array>
#include <fstream>
#include <vector>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	bool initialize(ID3D11Device*dx_device , HWND window_handle);
	void shutdown();
	bool renderShader(ID3D11DeviceContext* dx_device_context, int index_count, 
		DirectX::XMMATRIX world_matrix, 
		DirectX::XMMATRIX view_matrix, 
		DirectX::XMMATRIX projection_matrix);


private:
	struct MatrixBufferData
	{
		DirectX::XMMATRIX world_matrix;
		DirectX::XMMATRIX view_matrix;
		DirectX::XMMATRIX projection_matrix;
	};

	bool setShaderParameters(ID3D11DeviceContext* dx_device_context, DirectX::XMMATRIX world, DirectX::XMMATRIX projection, DirectX::XMMATRIX view);
	void printShaderError(ID3D10Blob* error_msg, HWND window_handle, WCHAR* filename);
	std::vector<BYTE> getByteArray(const char* filename);

	ID3D11VertexShader* m_vertex_shader;
	ID3D11PixelShader* m_pixel_shader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrix_buffer;
};

