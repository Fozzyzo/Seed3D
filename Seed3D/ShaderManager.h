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
		DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix, 
		DirectX::XMMATRIX projection_matrix, ID3D11ShaderResourceView* shader_resource_view,
		DirectX::XMFLOAT3 light_direction, DirectX::XMFLOAT4 ambient_light, DirectX::XMFLOAT4 diffuse_light,
		DirectX::XMFLOAT3 camera_position, DirectX::XMFLOAT4 specular_color, float specular_strength);


private:
	struct MatrixBufferData
	{
		DirectX::XMMATRIX world_matrix;
		DirectX::XMMATRIX view_matrix;
		DirectX::XMMATRIX projection_matrix;
	};

	struct CameraBufferData
	{
		DirectX::XMFLOAT3 camera_position;
		float padding;
	};

	struct LightBufferData
	{
		DirectX::XMFLOAT4 ambient_light;
		DirectX::XMFLOAT4 diffuse_color;
		DirectX::XMFLOAT3 light_direction;
		float specular_strength;
		DirectX::XMFLOAT4 specular_color;
	};

	bool setShaderParameters(ID3D11DeviceContext* dx_device_context, DirectX::XMMATRIX world, 
		DirectX::XMMATRIX projection, DirectX::XMMATRIX view, ID3D11ShaderResourceView* shader_resource_view,
		DirectX::XMFLOAT3 light_direction, DirectX::XMFLOAT4 ambient_light, DirectX::XMFLOAT4 diffuse_light,
		DirectX::XMFLOAT3 camera_position, DirectX::XMFLOAT4 specular_color, float specular_strength);

	void printShaderError(ID3D10Blob* error_msg, HWND window_handle, WCHAR* filename);
	std::vector<BYTE> getByteArray(const char* filename);

	ID3D11VertexShader* m_vertex_shader;
	ID3D11PixelShader* m_pixel_shader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrix_buffer;
	ID3D11Buffer* m_camera_buffer;
	ID3D11Buffer* m_light_buffer;
	ID3D11SamplerState* m_sampler_state;
};

