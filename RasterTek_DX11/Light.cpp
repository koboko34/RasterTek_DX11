#include "Light.h"

Light::Light()
{
}

Light::Light(const Light&)
{
}

Light::~Light()
{
}

void Light::SetAmbientColor(float r, float g, float b, float a)
{
	m_AmbientColor = DirectX::XMFLOAT4(r, g, b, a);
}

void Light::SetDiffuseColor(float r, float g, float b, float a)
{
	m_DiffuseColor = DirectX::XMFLOAT4(r, g, b, a);
}

void Light::SetDirection(float x, float y, float z)
{
	m_Direction = DirectX::XMFLOAT3(x, y, z);
}

void Light::SetSpecularColor(float r, float g, float b, float a)
{
	m_SpecularColor = DirectX::XMFLOAT4(r, g, b, a);
}

void Light::SetSpecularPower(float Power)
{
	m_SpecularPower = Power;
}
