#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	bool initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* filename);
	void destroy();
	void render(ID3D11DeviceContext* dx_device_context);
	int getIndexCount();
	ID3D11ShaderResourceView* getTexture();

private:
	bool initializeBuffers(ID3D11Device* dx_device);
	void destroyBuffers();
	void renderBuffers(ID3D11DeviceContext* dx_device_context);
	bool loadTexture(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char*);
	void releaseTexture();

	struct VertexData
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texture_coords;
	};

	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;
	int m_vertex_count;
	int m_index_count;

	Texture* m_texture;
};

