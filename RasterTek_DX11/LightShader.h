#pragma once

#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

const int NUM_LIGHTS = 4;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <fstream>

class LightShader
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

	struct LightColorBufferType
	{
		DirectX::XMFLOAT4 DiffuseColor[NUM_LIGHTS];
	};

	struct LightPositionBufferType
	{
		DirectX::XMFLOAT4 LightPosition[NUM_LIGHTS];
	};

public:
	LightShader();
	LightShader(const LightShader&);
	~LightShader();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int IndexCount, DirectX::XMMATRIX World, DirectX::XMMATRIX View, DirectX::XMMATRIX Proj, ID3D11ShaderResourceView* Texture, DirectX::XMFLOAT4* DiffuseColors, DirectX::XMFLOAT4* LightPositions);

private:
	bool InitialiseShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX World, DirectX::XMMATRIX View, DirectX::XMMATRIX Proj, ID3D11ShaderResourceView* Texture, DirectX::XMFLOAT4* DiffuseColors, DirectX::XMFLOAT4* LightPositions);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_Layout;
	ID3D11Buffer* m_MatrixBuffer;
	ID3D11Buffer* m_LightColorsBuffer;
	ID3D11Buffer* m_LightPositionsBuffer;
	ID3D11SamplerState* m_SampleState;
};

#endif
