#include "Camera.h"

Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionY = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

Camera::~Camera()
{
}

void Camera::setPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void Camera::setRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

DirectX::XMFLOAT3 Camera::getPosition()
{
	return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 Camera::getRotation()
{
	return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::render()
{
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR camera_position;
	DirectX::XMVECTOR camera_focus_point;
	DirectX::XMMATRIX rotation_matrix;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	up = DirectX::XMLoadFloat3(new DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
	camera_position = DirectX::XMLoadFloat3(new DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ));
	camera_focus_point = DirectX::XMLoadFloat3(new DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	camera_focus_point = DirectX::XMVector3TransformCoord(camera_focus_point, rotation_matrix);
	up = DirectX::XMVector3TransformCoord(up, rotation_matrix);

	camera_focus_point = DirectX::XMVectorAdd(camera_position, camera_focus_point);

	return;
}

void Camera::getViewMatrix(DirectX::XMMATRIX& view_matrix)
{
	view_matrix = m_view_matrix;
	return;
}
