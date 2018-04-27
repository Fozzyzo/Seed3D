#pragma once
#include <DirectXMath.h>
#include "KeyboardPresses.h"

class Camera
{
public:
	Camera();
	~Camera();
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void addRotation(float x, float y, float z);
	void setVelocity(float x, float y, float z);
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

	float m_velocityX;
	float m_velocityY;
	float m_velocityZ;
	DirectX::XMMATRIX m_view_matrix;
};

