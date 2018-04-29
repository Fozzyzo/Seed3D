#include "Material.h"



Material::Material()
{
	m_targa_data = 0;
	m_material[0] = 0;
	m_material[1] = 0;
	m_material[2] = 0;
}

Material::~Material()
{
}

bool Material::initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* color_texture_filename, char* normal_texture_filename, char* specular_texture_filename)
{
	int height;
	int width;
	D3D11_TEXTURE2D_DESC texture_description;
	unsigned int row_pitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_description;

	if (!loadTarga(color_texture_filename, width, height))
	{
		return false;
	}

	ZeroMemory(&texture_description, sizeof(texture_description));

	texture_description.Height = height;
	texture_description.Width = width;
	texture_description.MipLevels = 0;
	texture_description.ArraySize = 1;
	texture_description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_description.SampleDesc.Count = 1;
	texture_description.SampleDesc.Quality = 0;
	texture_description.Usage = D3D11_USAGE_DEFAULT;
	texture_description.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texture_description.CPUAccessFlags = 0;
	texture_description.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(dx_device->CreateTexture2D(&texture_description, NULL, &m_texture)))
	{
		return false;
	}

	row_pitch = (width * 4) * sizeof(unsigned char);
	dx_device_context->UpdateSubresource(m_texture, 0, NULL, m_targa_data, row_pitch, 0);

	//Load normal texture data

	if (!loadTarga(normal_texture_filename, width, height))
	{
		return false;
	}

	if (FAILED(dx_device->CreateTexture2D(&texture_description, NULL, &m_normal)))
	{
		return false;
	}
	
	row_pitch = (width * 4) * sizeof(unsigned char);
	dx_device_context->UpdateSubresource(m_normal, 0, NULL, m_targa_data, row_pitch, 0);

	//Load specular texture data

	if (!loadTarga(specular_texture_filename, width, height))
	{
		return false;
	}

	if (FAILED(dx_device->CreateTexture2D(&texture_description, NULL, &m_specular)))
	{
		return false;
	}

	row_pitch = (width * 4) * sizeof(unsigned char);
	dx_device_context->UpdateSubresource(m_specular, 0, NULL, m_targa_data, row_pitch, 0);

	shader_resource_view_description.Format = texture_description.Format;
	shader_resource_view_description.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_description.Texture2D.MostDetailedMip = 0;
	shader_resource_view_description.Texture2D.MipLevels = -1;

	if (FAILED(dx_device->CreateShaderResourceView(m_texture, &shader_resource_view_description, &m_material[0])))
	{
		return false;
	}

	dx_device_context->GenerateMips(m_material[0]);

	if (FAILED(dx_device->CreateShaderResourceView(m_normal, &shader_resource_view_description, &m_material[1])))
	{
		return false;
	}

	dx_device_context->GenerateMips(m_material[1]);

	if (FAILED(dx_device->CreateShaderResourceView(m_specular, &shader_resource_view_description, &m_material[2])))
	{
		return false;
	}

	dx_device_context->GenerateMips(m_material[2]);


	delete[] m_targa_data;
	m_targa_data = 0;

	return true;
}

void Material::destroy()
{
	if (m_material[2])
	{
		m_material[2]->Release();
		m_material[2] = 0;
	}

	if (m_material[1])
	{
		m_material[1]->Release();
		m_material[1] = 0;
	}

	if (m_material[0])
	{
		m_material[0]->Release();
		m_material[0] = 0;
	}

	if (m_normal)
	{
		m_normal->Release();
		m_normal = 0;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

ID3D11ShaderResourceView** Material::getMaterial()
{
	return m_material;
}

bool Material::loadTarga(char* filename, int& width, int& height)
{
	int error;
	int bpp;
	int image_size;
	int index;
	int i, j, k;
	FILE* file;
	unsigned int count;
	TargaData targa_file_data;
	unsigned char* targa_image;

	error = fopen_s(&file, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = (unsigned int)fread(&targa_file_data, sizeof(TargaData), 1, file);

	if (count != 1)
	{
		return false;
	}

	height = (int)targa_file_data.height;
	width = (int)targa_file_data.width;
	bpp = (int)targa_file_data.bpp;

	if (bpp != 32)
	{
		return false;
	}

	image_size = width * height * 4;

	targa_image = new unsigned char[image_size];

	count = (unsigned int)fread(targa_image, 1, image_size, file);

	if (count != image_size)
	{
		return false;
	}

	if (fclose(file))
	{
		return false;
	}

	m_targa_data = new unsigned char[image_size];

	index = 0;
	k = (width * height * 4) - (width * 4);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			m_targa_data[index + 0] = targa_image[k + 2];
			m_targa_data[index + 1] = targa_image[k + 1];
			m_targa_data[index + 2] = targa_image[k + 0];
			m_targa_data[index + 3] = targa_image[k + 3];

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	delete[] targa_image;
	targa_image = 0;

	return true;
}

