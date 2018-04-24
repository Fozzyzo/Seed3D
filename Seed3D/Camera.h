#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	~Camera();
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRotation();
	void render();
	void getViewMatrix(DirectX::XMMATRIX& view_matrix);

private:
	float m_positionX;
	float m_positionY;
	float m_positionZ;

	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;
	DirectX::XMMATRIX m_view_matrix;
};

