#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::Initialize(float width, float height, float x, float y, float z, float pitch, float yaw, float roll) {
	position = XMVectorSet(x, y, z, 0.0f);
	rotation.x = pitch;
	rotation.y = yaw;
	rotation.z = roll;
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	XMVECTOR lookAt = XMVector3Transform(forward, rotationMatrix);
	lookAt += position;
	XMVECTOR newUp = XMVector3Transform(up, rotationMatrix);

	viewMatirx = XMMatrixLookAtLH(position, lookAt, newUp);
	projectionMatrix = XMMatrixPerspectiveFovLH(0.4f * 3.14f, width / height, nearZ, 1000.0f);
}