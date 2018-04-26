#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	bool initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* texture_filename, char* mesh_filename);
	void destroy();
	void render(ID3D11DeviceContext* dx_device_context);
	int getIndexCount();
	ID3D11ShaderResourceView* getTexture();

private:
	bool initializeBuffers(ID3D11Device* dx_device);
	void destroyBuffers();
	void renderBuffers(ID3D11DeviceContext* dx_device_context);
	bool loadTexture(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* filename);
	bool loadObj(char* filename);
	void releaseTexture();

	struct VertexData
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texture_coords;
	};

	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;
	std::vector<DirectX::XMFLOAT3> m_vertices;
	std::vector<DirectX::XMFLOAT2> m_uvs;
	std::vector<DirectX::XMFLOAT3> m_normals;
	/*
		Indices :
		[0] = vertex 
		[1] = uv
		[2] = normal
	*/
	std::vector<std::vector<int>> m_indices;

	Texture* m_texture;
};

