#pragma once

#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include <d3d11.h>

#include <stdio.h>

class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char Data1[12];
		unsigned short Width;
		unsigned short Height;
		unsigned char bpp;
		unsigned char Data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

private:
	bool LoadTarga32Bit(char*);

private:
	unsigned char* m_TargaData;
	ID3D11Texture2D* m_Texture;
	ID3D11ShaderResourceView* m_TextureView;
	int m_Width, m_Height;

public:
	ID3D11ShaderResourceView* GetTexture() const { return m_TextureView; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
};

#endif
