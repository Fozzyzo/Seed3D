#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class Mesh
{
public:
	Mesh();
	~Mesh();
	bool initialize(ID3D11Device* dx_device);
	void destroy();
	void render(ID3D11DeviceContext* dx_device_context);
	int getIndexCount();

private:
	bool initializeBuffers(ID3D11Device* dx_device);
	void destroyBuffers();
	void renderBuffers(ID3D11DeviceContext* dx_device_context);


	struct VertexData
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;
	int m_vertex_count;
	int m_index_count;

};

