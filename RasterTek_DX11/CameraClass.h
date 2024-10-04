#pragma once

#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include <DirectXMath.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void Render();

private:
	float m_PositionX, m_PositionY, m_PositionZ;
	float m_RotationX, m_RotationY, m_RotationZ;
	DirectX::XMMATRIX m_ViewMatrix;

public:
	DirectX::XMFLOAT3 GetPosition() const { return DirectX::XMFLOAT3(m_PositionX, m_PositionY, m_PositionZ); }
	DirectX::XMFLOAT3 GetRotation() const { return DirectX::XMFLOAT3(m_RotationX, m_RotationY, m_RotationZ); }
	void GetViewMatrix(DirectX::XMMATRIX& ViewMatrix) {	ViewMatrix = m_ViewMatrix; }
};

#endif
