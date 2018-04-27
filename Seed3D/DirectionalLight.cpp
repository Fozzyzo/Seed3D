#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
{
}


DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setAmbientLight(float red, float green, float blue, float alpha)
{
	m_ambient_light = DirectX::XMFLOAT4(red, green, blue, alpha);
}

void DirectionalLight::setDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuse_color = DirectX::XMFLOAT4(red, green, blue, alpha);
	return;
}

void DirectionalLight::setDirection(float x, float y, float z)
{
	m_direction = DirectX::XMFLOAT3(x, y, z);
	return;
}

void DirectionalLight::setSpecularColor(float red, float green, float blue, float alpha)
{
	m_specular_color = DirectX::XMFLOAT4(red, green, blue, alpha);
	return;
}

void DirectionalLight::setSpecularPower(float specular_strength)
{
	m_specular_strength = specular_strength;
	return;
}

DirectX::XMFLOAT4 DirectionalLight::getAmbientLight()
{
	return m_ambient_light;
}

DirectX::XMFLOAT4 DirectionalLight::getDiffuseColor()
{
	return m_diffuse_color;
}

DirectX::XMFLOAT3 DirectionalLight::getDirection()
{
	return m_direction;
}

DirectX::XMFLOAT4 DirectionalLight::getSpecularColor()
{
	return m_specular_color;
}

float DirectionalLight::getSpecularStrength()
{
	return m_specular_strength;
}
