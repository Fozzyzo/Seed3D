#pragma once
#include <d3d11.h>
#include <stdio.h>

class Texture
{
public:
	Texture();
	~Texture();
	bool initialize(ID3D11Device* dx_device, ID3D11DeviceContext* dx_device_context, char* filename);
	void shutdown();
	ID3D11ShaderResourceView* getTexture();

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
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_texture_view;
};

