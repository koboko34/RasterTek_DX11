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

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	DirectX::XMFLOAT4 GetAmbientColor() const { return m_AmbientColor; }
	DirectX::XMFLOAT4 GetDiffuseColor() const { return m_DiffuseColor; }
	DirectX::XMFLOAT3 GetDirection() const { return m_Direction; }
	DirectX::XMFLOAT4 GetSpecularColor() const { return m_SpecularColor; }
	float GetSpecularPower() const { return m_SpecularPower; }

private:
	DirectX::XMFLOAT4 m_AmbientColor;
	DirectX::XMFLOAT4 m_DiffuseColor;
	DirectX::XMFLOAT3 m_Direction;
	DirectX::XMFLOAT4 m_SpecularColor;
	float m_SpecularPower;
};

#endif
