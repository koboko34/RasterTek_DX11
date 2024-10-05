#pragma once

#ifndef MODELCLASS_H
#define MODELCLASS_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "TextureClass.h"

class ModelClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 TexCoord;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	unsigned int GetIndexCount() const { return m_IndexCount; }
	ID3D11ShaderResourceView* GetTexture() const { return m_Texture->GetTexture(); };

private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	unsigned int m_VertexCount, m_IndexCount;
	TextureClass* m_Texture;

};

#endif
