#pragma once
#include <DirectXMath.h>
class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	void setAmbientLight(float red, float green, float blue, float alpha);
	void setDiffuseColor(float red, float green, float blue, float alpha);
	void setDirection(float x, float y, float z);
	void setSpecularColor(float red, float green, float blue, float alpha);
	void setSpecularPower(float specular_strength);

	DirectX::XMFLOAT4 getAmbientLight();
	DirectX::XMFLOAT4 getDiffuseColor();
	DirectX::XMFLOAT3 getDirection();
	DirectX::XMFLOAT4 getSpecularColor();
	float getSpecularStrength();

private:
	DirectX::XMFLOAT4 m_ambient_light;
	DirectX::XMFLOAT4 m_diffuse_color;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMFLOAT4 m_specular_color;
	float m_specular_strength;
};

