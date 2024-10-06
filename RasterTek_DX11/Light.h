#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <DirectXMath.h>

class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	DirectX::XMFLOAT4 GetDiffuseColor() const { return m_DiffuseColor; }
	DirectX::XMFLOAT3 GetDirection() const { return m_Direction; }

private:
	DirectX::XMFLOAT4 m_DiffuseColor;
	DirectX::XMFLOAT3 m_Direction;
};

#endif
