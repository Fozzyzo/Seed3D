#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	m_vertex_shader = 0;
	m_pixel_shader = 0;
	m_layout = 0;
	m_matrix_buffer = 0;
	m_camera_buffer = 0;
	m_light_buffer = 0;
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::initialize(ID3D11Device * dx_device, HWND window_handle)
{
	ID3D10Blob* error_msg;
	ID3D10Blob* vertex_shader_buffer;
	ID3D10Blob* pixel_shader_buffer;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[4];
	unsigned int element_count;
	D3D11_BUFFER_DESC matrix_buffer_description;
	D3D11_BUFFER_DESC camera_buffer_description;
	D3D11_BUFFER_DESC light_buffer_description;
	D3D11_SAMPLER_DESC texture_sampler_description;

	error_msg = 0;
	vertex_shader_buffer = 0;
	pixel_shader_buffer = 0;
	
	if (FAILED(D3DCompileFromFile(L"shaders/VertexShader.hlsl", NULL, NULL, "vertexMain", "vs_5_0", 
				D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertex_shader_buffer, &error_msg)))
	{
		if (error_msg)
		{
			printShaderError(error_msg, window_handle, (WCHAR*)"VertexShader.hlsl");
		}

		else
		{
			MessageBox(window_handle, "Missing vertex shaderfile", "Error", MB_OK);
		}

		return false;
	}
	
	if (FAILED(D3DCompileFromFile(L"shaders/PixelShader.hlsl", NULL, NULL, "pixelMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixel_shader_buffer, &error_msg)))
	{
		if (error_msg)
		{
			printShaderError(error_msg, window_handle, (WCHAR*)"PixelShader.hlsl");
		}

		else
		{
			MessageBox(window_handle, "Missing pixel shared file", "Error", MB_OK);
		}

		return false;
	}
	
	if (FAILED(dx_device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), NULL, &m_vertex_shader)))
	{
		return false;
	}

	if (FAILED(dx_device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), NULL, &m_pixel_shader)))
	{
		return false;
	}

	polygon_layout[0].SemanticName = "POSITION";
	polygon_layout[0].SemanticIndex = 0;
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[0].InputSlot = 0;
	polygon_layout[0].AlignedByteOffset = 0;
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[0].InstanceDataStepRate = 0;

	polygon_layout[1].SemanticName = "COLOR";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

	polygon_layout[2].SemanticName = "TEXCOORD";
	polygon_layout[2].SemanticIndex = 0;
	polygon_layout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygon_layout[2].InputSlot = 0;
	polygon_layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[2].InstanceDataStepRate = 0;
	
	polygon_layout[3].SemanticName = "NORMAL";
	polygon_layout[3].SemanticIndex = 0;
	polygon_layout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[3].InputSlot = 0;
	polygon_layout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[3].InstanceDataStepRate = 0;

	element_count = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

	if (FAILED(dx_device->CreateInputLayout(polygon_layout, element_count, vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), &m_layout)))
	{
		return false;
	}

	vertex_shader_buffer->Release();
	vertex_shader_buffer = 0;

	pixel_shader_buffer->Release();
	pixel_shader_buffer = 0;

	matrix_buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_description.ByteWidth = sizeof(MatrixBufferData);
	matrix_buffer_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_description.MiscFlags = 0;
	matrix_buffer_description.StructureByteStride = 0;

	if (FAILED(dx_device->CreateBuffer(&matrix_buffer_description, NULL, &m_matrix_buffer)))
	{
		return false;
	}

	camera_buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	camera_buffer_description.ByteWidth = sizeof(CameraBufferData);
	camera_buffer_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	camera_buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	camera_buffer_description.MiscFlags = 0;
	camera_buffer_description.StructureByteStride = 0;

	if (FAILED(dx_device->CreateBuffer(&camera_buffer_description, NULL, &m_camera_buffer)))
	{
		return false;
	}

	light_buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	light_buffer_description.ByteWidth = sizeof(LightBufferData);
	light_buffer_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	light_buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	light_buffer_description.MiscFlags = 0;
	light_buffer_description.StructureByteStride = 0;

	if (FAILED(dx_device->CreateBuffer(&light_buffer_description, NULL, &m_light_buffer)))
	{
		return false;
	}

	texture_sampler_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	texture_sampler_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	texture_sampler_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	texture_sampler_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	texture_sampler_description.MipLODBias = 0.0f;
	texture_sampler_description.MaxAnisotropy = 1;
	texture_sampler_description.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	texture_sampler_description.BorderColor[0] = 0;
	texture_sampler_description.BorderColor[1] = 0;
	texture_sampler_description.BorderColor[2] = 0;
	texture_sampler_description.BorderColor[3] = 0;
	texture_sampler_description.MinLOD = 0;
	texture_sampler_description.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(dx_device->CreateSamplerState(&texture_sampler_description, &m_sampler_state)))
	{
		return false;
	}

	return true;
}

void ShaderManager::shutdown()
{
	if (m_light_buffer)
	{
		m_light_buffer->Release();
		m_light_buffer = 0;
	}

	if (m_camera_buffer)
	{
		m_camera_buffer->Release();
		m_camera_buffer = 0;
	}

	if (m_matrix_buffer)
	{
		m_matrix_buffer->Release();
		m_matrix_buffer = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_vertex_shader)
	{
		m_vertex_shader->Release();
		m_vertex_shader = 0;
	}

	if (m_pixel_shader)
	{
		m_pixel_shader->Release();
		m_pixel_shader = 0;
	}

	if (m_sampler_state)
	{
		m_sampler_state->Release();
		m_sampler_state = 0;
	}

	return;
}

bool ShaderManager::setShaderParameters(ID3D11DeviceContext* dx_device_context, 
	DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection,
	ID3D11ShaderResourceView* shader_resource_view, DirectX::XMFLOAT3 light_direction,
	DirectX::XMFLOAT4 ambient_light, DirectX::XMFLOAT4 diffure_color, 
	DirectX::XMFLOAT3 camera_position, DirectX::XMFLOAT4 specular_color, float specular_strength)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	MatrixBufferData* matrix_data;
	LightBufferData* light_data;
	CameraBufferData* camera_data;
	unsigned int buffer_count;

	world = DirectX::XMMatrixTranspose(world);
	view = DirectX::XMMatrixTranspose(view);
	projection = DirectX::XMMatrixTranspose(projection);

	if (FAILED(dx_device_context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)))
	{
		return false;
	}

	matrix_data = (MatrixBufferData*)mapped_resource.pData;

	matrix_data->world_matrix = world;
	matrix_data->view_matrix = view;
	matrix_data->projection_matrix = projection;

	dx_device_context->Unmap(m_matrix_buffer, 0);

	buffer_count = 0;
	dx_device_context->VSSetConstantBuffers(buffer_count, 1, &m_matrix_buffer);
	dx_device_context->PSSetShaderResources(0, 1, &shader_resource_view);

	if (FAILED(dx_device_context->Map(m_camera_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)))
	{
		return false;
	}

	camera_data = (CameraBufferData*)mapped_resource.pData;

	camera_data->camera_position = camera_position;
	camera_data->padding = 0.0f;

	dx_device_context->Unmap(m_camera_buffer, 0);

	buffer_count = 1;
	dx_device_context->VSSetConstantBuffers(buffer_count, 1, &m_camera_buffer);
	dx_device_context->PSSetShaderResources(0, 1, &shader_resource_view);

	if (FAILED(dx_device_context->Map(m_light_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)))
	{
		return false;
	}

	light_data = (LightBufferData*)mapped_resource.pData;

	light_data->ambient_light = ambient_light;
	light_data->diffuse_color = diffure_color;
	light_data->light_direction = light_direction;
	light_data->specular_strength = specular_strength;
	light_data->specular_color = specular_color;

	dx_device_context->Unmap(m_light_buffer, 0);

	buffer_count = 0;
	dx_device_context->PSSetConstantBuffers(buffer_count, 1, &m_light_buffer);
	return true;
}

bool ShaderManager::renderShader(ID3D11DeviceContext* dx_device_context, int index_count,
	DirectX::XMMATRIX world_matrix, DirectX::XMMATRIX view_matrix, DirectX::XMMATRIX projection_matrix,
	ID3D11ShaderResourceView* shader_resource_view, DirectX::XMFLOAT3 light_direction, DirectX::XMFLOAT4 ambient_light,
	DirectX::XMFLOAT4 diffuse_light, DirectX::XMFLOAT3 camera_position, DirectX::XMFLOAT4 specular_color, float specular_strength)
{
	if (!setShaderParameters(dx_device_context, world_matrix, view_matrix, projection_matrix, 
		shader_resource_view, light_direction, ambient_light, diffuse_light, camera_position,
		specular_color, specular_strength))
	{
		return false;
	}

	dx_device_context->IASetInputLayout(m_layout);
	dx_device_context->VSSetShader(m_vertex_shader, NULL, 0);
	dx_device_context->PSSetShader(m_pixel_shader, NULL, 0);
	dx_device_context->PSGetSamplers(0, 1, &m_sampler_state);
	dx_device_context->DrawIndexed(index_count, 0, 0);

	return true;
}

void ShaderManager::printShaderError(ID3D10Blob* error_msg, HWND window_handle, WCHAR* filename)
{
	char* compile_errors;
	unsigned long buffer_size;
	int i;
	std::ofstream filestream;

	compile_errors = (char*)(error_msg->GetBufferPointer());
	buffer_size = error_msg->GetBufferSize();

	filestream.open("shader-error.txt");

	for (i = 0; i<buffer_size; i++)
	{
		filestream << compile_errors[i];
	}

	filestream.close();

	error_msg->Release();
	error_msg = 0;
	MessageBox(window_handle, (LPCSTR)"Error compiling shader.  Check shader-error.txt for message.", (LPCSTR)filename, MB_OK);

	return;
}

std::vector<BYTE> ShaderManager::getByteArray(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);
	file.unsetf(std::ios::skipws);

	std::streampos file_size;

	file.seekg(0, std::ios::end);
	file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<BYTE> byte_array;
	byte_array.reserve(file_size);
	byte_array.insert(byte_array.begin(), std::istream_iterator<BYTE>(file),
		std::istream_iterator<BYTE>());

	return byte_array;
}


