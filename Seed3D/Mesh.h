#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Material.h"

class Mesh
{

public:
	Mesh();
	~Mesh();
	bool initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* mesh_filename, char* texture_filename, char* normals_filename, char* specular_filename);
	void destroy();
	void render(ID3D11DeviceContext* dx_device_context);
	int getIndexCount();
	void setPosition(float x, float y, float z);
	DirectX::XMMATRIX getPosition();
	ID3D11ShaderResourceView** getMaterial();
	
private:

	friend DirectX::XMFLOAT3 operator+ (DirectX::XMFLOAT3& f1, DirectX::XMFLOAT3& f2);
	friend DirectX::XMFLOAT2 operator+ (DirectX::XMFLOAT2& f1, DirectX::XMFLOAT2& f2);
	friend DirectX::XMFLOAT3 operator- (DirectX::XMFLOAT3& f1, DirectX::XMFLOAT3& f2);
	friend DirectX::XMFLOAT2 operator- (DirectX::XMFLOAT2& f1, DirectX::XMFLOAT2& f2);
	friend DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3& f1, float& f2);

	bool initializeBuffers(ID3D11Device* dx_device);
	void destroyBuffers();
	void renderBuffers(ID3D11DeviceContext* dx_device_context);
	bool loadMaterial(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* texture_filename, char* normals_filename, char* specular_filename);
	bool loadObj(char* filename);
	void releaseMaterial();

	struct VertexData
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texture_coords;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 bitangent;
	};

	void calculateTangents(struct VertexData*& vertices);

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
	Material* m_material;

	DirectX::XMMATRIX m_position;
};

