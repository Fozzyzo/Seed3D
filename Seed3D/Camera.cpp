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
	DirectX::XMFLOAT3 up_float;

	DirectX::XMVECTOR camera_position;
	DirectX::XMFLOAT3 camera_position_float;

	DirectX::XMVECTOR camera_focus_point;
	DirectX::XMFLOAT3 camera_focus_point_float;

	DirectX::XMMATRIX rotation_matrix;

	up_float.x = 0.0f;
	up_float.y = 1.0f;
	up_float.z = 0.0f;
	up = DirectX::XMLoadFloat3(&up_float);

	camera_position_float.x = m_positionX;
	camera_position_float.y = m_positionY;
	camera_position_float.z = m_positionZ;
	camera_position = DirectX::XMLoadFloat3(&camera_position_float);

	camera_focus_point_float.x = 0.0f;
	camera_focus_point_float.y = 0.0f;
	camera_focus_point_float.z = 1.0f;
	camera_focus_point = DirectX::XMLoadFloat3(&camera_focus_point_float);

	float pitch = m_rotationX * 0.0174532925f;
	float yaw = m_rotationY * 0.0174532925f;
	float roll = m_rotationZ * 0.0174532925f;

	rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	camera_focus_point = DirectX::XMVector3TransformCoord(camera_focus_point, rotation_matrix);
	up = DirectX::XMVector3TransformCoord(up, rotation_matrix);

	camera_focus_point = DirectX::XMVectorAdd(camera_position, camera_focus_point);

	m_view_matrix = DirectX::XMMatrixLookAtLH(camera_position, camera_focus_point, up);
	
	return;
}

void Camera::getViewMatrix(DirectX::XMMATRIX& view_matrix)
{
	view_matrix = m_view_matrix;
	return;
}
