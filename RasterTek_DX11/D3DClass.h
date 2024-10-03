#pragma once

#ifndef D3DCLASS_H
#define D3DCLASS_H

// LINKING //
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialise(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	void GetVideoCardInfo(char*, int&);

	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool m_VSync_Enabled;
	int m_VideoCardMemory;
	char m_VideoCardDescription[128];
	IDXGISwapChain* m_SwapChain;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11Texture2D* m_DepthStencilBuffer;
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11DepthStencilView* m_DepthStencilView;
	ID3D11RasterizerState* m_RasterState;
	DirectX::XMMATRIX m_ProjectionMatrix;
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_OrthoMatrix;
	D3D11_VIEWPORT m_Viewport;

public:
	ID3D11Device* GetDevice() const { return m_Device; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }

	DirectX::XMMATRIX GetProjectionMatrix() const { return m_ProjectionMatrix; }
	DirectX::XMMATRIX GetWorldMatrix() const { return m_WorldMatrix; }
	DirectX::XMMATRIX GetOrthoMatrix() const { return m_OrthoMatrix; }
};

#endif
