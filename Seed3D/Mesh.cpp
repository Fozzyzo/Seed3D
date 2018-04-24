#include "Mesh.h"



Mesh::Mesh()
{
	m_vertex_buffer = 0;
	m_index_buffer = 0;
}

Mesh::~Mesh()
{
}

bool Mesh::initialize(ID3D11Device* dx_device)
{
	if (!initializeBuffers(dx_device))
	{ return false; }

	return true;
}

void Mesh::destroy()
{
	destroyBuffers();

	return;
}

void Mesh::render(ID3D11DeviceContext* dx_device_context)
{
	renderBuffers(dx_device_context);
}

int Mesh::getIndexCount()
{
	return m_index_count;
}

bool Mesh::initializeBuffers(ID3D11Device* dx_device)
{
	VertexData* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertex_buffer_description;
	D3D11_BUFFER_DESC index_buffer_description;
	D3D11_SUBRESOURCE_DATA vertex_data;
	D3D11_SUBRESOURCE_DATA index_data;
	m_vertex_count = 3;
	m_index_count = 3;

	vertices = new VertexData[m_vertex_count];
	indices = new unsigned long[m_index_count];

	vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	indices[0] = 0;

	vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	indices[1] = 1;

	vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	indices[2] = 2;

	vertex_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_description.ByteWidth = sizeof(VertexData) * m_vertex_count;
	vertex_buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_description.CPUAccessFlags = 0;
	vertex_buffer_description.MiscFlags = 0;
	vertex_buffer_description.StructureByteStride = 0;

	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	if (FAILED(dx_device->CreateBuffer(&vertex_buffer_description, &vertex_data, &m_vertex_buffer)))
	{
		return false;
	}

	index_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_description.ByteWidth = sizeof(unsigned long) * m_index_count;
	index_buffer_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_description.CPUAccessFlags = 0;
	index_buffer_description.MiscFlags = 0;
	index_buffer_description.StructureByteStride = 0;

	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	if (FAILED(dx_device->CreateBuffer(&index_buffer_description, &index_data, &m_index_buffer)))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Mesh::destroyBuffers()
{
	if(m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = 0;
	}

	if (m_vertex_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = 0;
	}

	return;
}

void Mesh::renderBuffers(ID3D11DeviceContext* dx_device_context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexData);
	offset = 0;

	dx_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	dx_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
	dx_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
