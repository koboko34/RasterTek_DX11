#pragma once

#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

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

	struct LightBufferType
	{
		DirectX::XMFLOAT4 DiffuseColor;
		DirectX::XMFLOAT3 LightDirection;
		float Padding;
	};

public:
	LightShader();
	LightShader(const LightShader&);
	~LightShader();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*, DirectX::XMFLOAT3, DirectX::XMFLOAT4);

private:
	bool InitialiseShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*, DirectX::XMFLOAT3, DirectX::XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_Layout;
	ID3D11Buffer* m_MatrixBuffer;
	ID3D11Buffer* m_LightBuffer;
	ID3D11SamplerState* m_SampleState;
};

#endif
