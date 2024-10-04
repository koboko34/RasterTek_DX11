#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_PositionX = 0.0f;
	m_PositionY = 0.0f;
	m_PositionZ = 0.0f;

	m_RotationX = 0.0f;
	m_RotationY = 0.0f;
	m_RotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass&)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_PositionX = x;
	m_PositionY = y;
	m_PositionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_RotationX = x;
	m_RotationY = y;
	m_RotationZ = z;
}

void CameraClass::Render()
{
	DirectX::XMFLOAT3 Up, Position, LookAt;
	DirectX::XMVECTOR UpVector, PositionVector, LookAtVector;
	DirectX::XMMATRIX RotationMatrix;
	float Yaw, Pitch, Roll;

	Up.x = 0.f;
	Up.y = 1.f;
	Up.z = 0.f;

	UpVector = DirectX::XMLoadFloat3(&Up);

	Position.x = m_PositionX;
	Position.y = m_PositionY;
	Position.z = m_PositionZ;

	PositionVector = DirectX::XMLoadFloat3(&Position);

	LookAt.x = 0.f;
	LookAt.y = 0.f;
	LookAt.z = 1.f;

	LookAtVector = DirectX::XMLoadFloat3(&LookAt);

	Pitch = m_RotationX * 0.0174532925f;
	Yaw = m_RotationY * 0.0174532925f;
	Roll = m_RotationZ * 0.0174532925f;

	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	LookAtVector = DirectX::XMVector3TransformCoord(LookAtVector, RotationMatrix);
	UpVector = DirectX::XMVector3TransformCoord(UpVector, RotationMatrix);

	LookAtVector = DirectX::XMVectorAdd(PositionVector, LookAtVector);

	m_ViewMatrix = DirectX::XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector);
}
