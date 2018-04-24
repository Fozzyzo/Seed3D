#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	m_vertex_shader = 0;
	m_pixel_shader = 0;
	m_layout = 0;
	m_matrix_buffer = 0;
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::initialize(ID3D11Device * dx_device, HWND window_handle)
{
	ID3D10Blob* error_msg;
	ID3D10Blob* vertex_shader_buffer;
	ID3D10Blob* pixel_shader_buffer;
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
	unsigned int element_count;
	D3D11_BUFFER_DESC matrix_buffer_description;

	error_msg = 0;
	vertex_shader_buffer = 0;
	pixel_shader_buffer = 0;
	
	
	if (FAILED(D3DCompileFromFile(L"shaders/VertexShader.hlsl", NULL, NULL, "vertexMain", "vs_5_0", 
				D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertex_shader_buffer, &error_msg)))
	{
		if (error_msg)
		{
			//printShaderError(error_msg, window_handle, L"VertexShader.hlsl");
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
			//printShaderError(error_msg, window_handle, "PixelShader.hlsl");
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

	element_count = sizeof(polygon_layout) / sizeof(polygon_layout[1]);

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

	return true;
}

void ShaderManager::shutdown()
{
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

	return;
}

bool ShaderManager::setShaderParameters(ID3D11DeviceContext* dx_device_context, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	MatrixBufferData* matrix_data;
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
	return true;
}

bool ShaderManager::renderShader(ID3D11DeviceContext* dx_device_context, int index_count,
	DirectX::XMMATRIX world_matrix,
	DirectX::XMMATRIX view_matrix,
	DirectX::XMMATRIX projection_matrix)
{
	if (!setShaderParameters(dx_device_context, world_matrix, view_matrix, projection_matrix))
	{
		return false;
	}

	dx_device_context->IASetInputLayout(m_layout);
	dx_device_context->VSSetShader(m_vertex_shader, NULL, 0);
	dx_device_context->PSSetShader(m_pixel_shader, NULL, 0);

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


