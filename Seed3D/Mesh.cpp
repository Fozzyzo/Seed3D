#include "Mesh.h"

Mesh::Mesh()
{
	m_vertex_buffer = 0;
	m_index_buffer = 0;
	m_material = 0;
	m_vertices = {};
	m_uvs = {};
	m_normals = {};
}

Mesh::~Mesh()
{
}

DirectX::XMFLOAT3 operator+ (DirectX::XMFLOAT3& f1, DirectX::XMFLOAT3& f2)
{
	return DirectX::XMFLOAT3(f1.x + f2.x, f1.y + f2.y, f1.z + f2.z);
}

DirectX::XMFLOAT2 operator+ (DirectX::XMFLOAT2& f1, DirectX::XMFLOAT2& f2)
{
	return DirectX::XMFLOAT2(f1.x + f2.x, f1.y + f2.y);
}

DirectX::XMFLOAT3 operator- (DirectX::XMFLOAT3& f1, DirectX::XMFLOAT3& f2)
{
	return DirectX::XMFLOAT3(f1.x - f2.x, f1.y - f2.y, f1.z - f2.z);
}

DirectX::XMFLOAT2 operator-(DirectX::XMFLOAT2 & f1, DirectX::XMFLOAT2 & f2)
{
	return DirectX::XMFLOAT2(f1.x - f2.x, f1.y - f2.y);
}

DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3& f1, float& f2)
{
	return DirectX::XMFLOAT3(f1.x * f2, f1.y * f2, f1.z * f2);
}

bool Mesh::initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* mesh_filename, char* texture_filename, char* normal_texture_filename, float specularity)
{
	if (!loadObj(mesh_filename))
	{
		return false;
	}

	if (!initializeBuffers(dx_device))
	{ 
		return false; 
	}

	if (!loadMaterial(dx_device, dx_device_context, texture_filename, normal_texture_filename, specularity))
	{
		return false;
	}

	return true;
}

void Mesh::destroy()
{
	releaseMaterial();
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

ID3D11ShaderResourceView** Mesh::getMaterial()
{
	return m_material->getMaterial();
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
		vertices[i].normal = m_normals[m_indices[i][2] - 1];
		indices[i] = i;
	}

	calculateTangents(vertices);

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
		m_vertex_buffer->Release();
		m_vertex_buffer = 0;
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

bool Mesh::loadMaterial(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* texture_filename, char* normals_filename, float specularity)
{
	m_material = new Material();

	if (!m_material->initialize(dx_device, dx_device_context, texture_filename, normals_filename, specularity))
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

void Mesh::releaseMaterial()
{
	if (m_material)
	{
		m_material->destroy();
		delete m_material;
		m_material = 0;
	}

	return;
}

void Mesh::calculateTangents(VertexData*& vertices)
{
	DirectX::XMFLOAT3 delta_position1;
	DirectX::XMFLOAT3 delta_position2;
	DirectX::XMFLOAT2 delta_uv1;
	DirectX::XMFLOAT2 delta_uv2;

	DirectX::XMFLOAT3 tmp1;
	DirectX::XMFLOAT3 tmp2;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
	float length;

	for (int i = 0; i < m_indices.size(); i += 3)
	{
		delta_position1 = vertices[i + 1].position - vertices[i].position;
		delta_position2 = vertices[i + 2].position - vertices[i].position;

		delta_uv1 = vertices[i + 1].texture_coords - vertices[i].texture_coords;
		delta_uv2 = vertices[i + 2].texture_coords - vertices[i].texture_coords;

		float r = 1.0f / ((delta_uv1.x * delta_uv2.y) - (delta_uv1.y * delta_uv2.x));
		
		//Calculate tangent
		tmp1 = delta_position1 * delta_uv2.y;
		tmp2 = delta_position2 * delta_uv1.y;
		tangent = tmp1 - tmp2;
		tangent = tangent * r;

		//Normalize
		length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
		tangent.x = tangent.x / length;
		tangent.y = tangent.y / length;
		tangent.z = tangent.z / length;

		vertices[i].tangent = tangent;
		vertices[i + 1].tangent = tangent;
		vertices[i + 2].tangent = tangent;

		//Calculate bitangent
		tmp1 = delta_position2 * delta_uv1.x;
		tmp2 = delta_position1 * delta_uv2.x;
		bitangent = tmp1 - tmp2;
		bitangent = bitangent * r;

		length = sqrt((bitangent.x * bitangent.x) + (bitangent.y * bitangent.y) + (bitangent.z * bitangent.z));
		bitangent.x = bitangent.x / length;
		bitangent.y = bitangent.y / length;
		bitangent.z = bitangent.z / length;

		vertices[i].bitangent = bitangent;
		vertices[i + 1].bitangent = bitangent;
		vertices[i + 2].bitangent = bitangent;
	}

	return;
}
