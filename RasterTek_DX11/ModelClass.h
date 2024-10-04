#pragma once

#ifndef MODELCLASS_H
#define MODELCLASS_H

#include <d3d11.h>
#include <DirectXMath.h>

class ModelClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialise(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	unsigned int GetIndexCount() const { return m_IndexCount; }

private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	unsigned int m_VertexCount, m_IndexCount;

};

#endif
