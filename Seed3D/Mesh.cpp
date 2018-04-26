#include "Mesh.h"



Mesh::Mesh()
{
	m_vertex_buffer = 0;
	m_index_buffer = 0;
	m_texture = 0;
	m_vertices = {};
	m_uvs = {};
	m_normals = {};
}

Mesh::~Mesh()
{
}

bool Mesh::initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* texture_filename, char* mesh_filename)
{
	if (!loadObj(mesh_filename))
	{
		return false;
	}

	if (!initializeBuffers(dx_device))
	{ 
		return false; 
	}

	if (!loadTexture(dx_device, dx_device_context, texture_filename))
	{
		return false;
	}

	return true;
}

void Mesh::destroy()
{
	releaseTexture();
	destroyBuffers();

	m_vertices.clear();
	m_indices.clear();
	m_normals.clear();
	m_indices.clear();

	return;
}

void Mesh::render(ID3D11DeviceContext* dx_device_context)
{
	renderBuffers(dx_device_context);
}

int Mesh::getIndexCount()
{
	return m_indices.size();
}

ID3D11ShaderResourceView * Mesh::getTexture()
{
	return m_texture->getTexture();
}

bool Mesh::initializeBuffers(ID3D11Device* dx_device)
{
	VertexData* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertex_buffer_description;
	D3D11_BUFFER_DESC index_buffer_description;
	D3D11_SUBRESOURCE_DATA vertex_data;
	D3D11_SUBRESOURCE_DATA index_data;

	vertices = new VertexData[m_indices.size()];
	indices = new unsigned long[m_indices.size()];

	for (int i = 0; i < m_indices.size(); i++)
	{
		vertices[i].position = m_vertices[m_indices[i][0] - 1];
		vertices[i].color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		vertices[i].texture_coords = m_uvs[m_indices[i][1] - 1];
		indices[i] = i;
	}

	vertex_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_description.ByteWidth = sizeof(VertexData) * m_indices.size();
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
	index_buffer_description.ByteWidth = sizeof(unsigned long) * m_indices.size();
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

bool Mesh::loadTexture(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* filename)
{
	m_texture = new Texture();

	if (!m_texture->initialize(dx_device, dx_device_context, filename))
	{
		return false;
	}

	return true;
}

bool Mesh::loadObj(char* filename)
{
	std::fstream filestream(filename);
	std::string line;

	//Count vertices
	while (std::getline(filestream, line)) 
	{
		std::istringstream line_stream(line);
		std::string line_prefix;
		std::string raw_indices;
		float x, y, z;


		if (!(line_stream >> line_prefix))
		{
			continue;
		}

		else if ((line_prefix.compare("v")) == 0)
		{
			line_stream >> x >> y >> z;
			m_vertices.push_back(DirectX::XMFLOAT3(x, y, z));
		}

		else if ((line_prefix.compare("vt") == 0))
		{
			line_stream >> x >> y;
			m_uvs.push_back(DirectX::XMFLOAT2(x, y));
		}

		else if ((line_prefix.compare("vn") == 0))
		{
			line_stream >> x >> y >> z;
			m_normals.push_back(DirectX::XMFLOAT3(x, y, z));
		}

		else if((line_prefix.compare("f") == 0))
		{
			while (line_stream >> raw_indices)
			{
				char delimiter = '/';
				std::istringstream indices(raw_indices);
				std::vector<int> tmp_indices;

				for (std::string token; std::getline(indices, token, delimiter);)
				{
					tmp_indices.push_back(std::stoi(token, nullptr, 0));
				}

				m_indices.push_back(tmp_indices);
				tmp_indices.clear();
			}
		}

		else
		{
			continue;
		}
	}

	//Reset filestream position
	filestream.clear();
	filestream.seekg(0);
	return true;
}

void Mesh::releaseTexture()
{
	if (m_texture)
	{
		m_texture->shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}
