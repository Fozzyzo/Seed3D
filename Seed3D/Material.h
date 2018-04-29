#pragma once
#include <d3d11.h>
#include <stdio.h>

class Material
{
public:
	Material();
	~Material();
	bool initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* color_texture_filename, char* normal_texture_filename, char* specular_texture_filename);
	void destroy();
	ID3D11ShaderResourceView** getMaterial();

private:

	bool loadTarga(char* filename, int& width, int& height);

	struct TargaData
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
	unsigned char* m_targa_data;
	float m_specularity;
	ID3D11Texture2D* m_texture;
	ID3D11Texture2D* m_normal;
	ID3D11Texture2D* m_specular;
	ID3D11ShaderResourceView* m_material[3];
};

