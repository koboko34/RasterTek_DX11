#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass&)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialise(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	char TextureFilename[128];
	
	m_Direct3D = new D3DClass();

	bool Result = m_Direct3D->Initialise(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!Result)
	{
		ShowCursor(true);
		MessageBox(hwnd, L"Could not initialise Direct3D!", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass();
	m_Camera->SetPosition(0.f, 0.f, -5.f);

	strcpy_s(TextureFilename, "Textures/stone01.tga");

	m_Model = new ModelClass();
	Result = m_Model->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), TextureFilename);
	if (!Result)
	{
		MessageBox(hwnd, L"Failed to initialise to model object!", L"Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShaderClass();
	Result = m_TextureShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!Result)
	{
		MessageBox(hwnd, L"Failed to initialise texture shader object!", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool ApplicationClass::Frame()
{
	bool Result = Render();
	if (!Result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render()
{
	DirectX::XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix;
	bool Result;
	
	m_Direct3D->BeginScene(0.f, 0.f, 0., 1.f);

	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(WorldMatrix);
	m_Camera->GetViewMatrix(ViewMatrix);
	m_Direct3D->GetProjectionMatrix(ProjectionMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	Result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTexture());
	if (!Result)
	{
		return false;
	}

	m_Direct3D->EndScene();
	
	return true;
}
