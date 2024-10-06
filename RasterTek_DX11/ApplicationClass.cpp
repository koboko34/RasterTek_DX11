#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass&)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialise(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	char ModelFilename[128];
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
	m_Camera->SetPosition(0.f, 0.f, -10.f);

	strcpy_s(ModelFilename, "Models/cube.txt");
	strcpy_s(TextureFilename, "Textures/stone01.tga");

	m_Model = new ModelClass();
	Result = m_Model->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), ModelFilename, TextureFilename);
	if (!Result)
	{
		MessageBox(hwnd, L"Failed to initialise to model object!", L"Error", MB_OK);
		return false;
	}

	m_LightShader = new LightShader();
	Result = m_LightShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!Result)
	{
		MessageBox(hwnd, L"Failed to initialise light shader object!", L"Error", MB_OK);
		return false;
	}

	m_Light = new Light();
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetDirection(0.f, 0.f, 1.f);

	return true;
}

void ApplicationClass::Shutdown()
{
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
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
	static float Rotation = 0.f;
	Rotation -= 0.0174532925f * 0.5f;
	if (Rotation < 0.f)
	{
		Rotation += 360.f;
	}

	bool Result = Render(Rotation);
	if (!Result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render(float Rotation)
{
	DirectX::XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix, RotateMatrix, TranslateMatrix, ScaleMatrix, srMatrix;
	bool Result;
	
	m_Direct3D->BeginScene(0.f, 0.f, 0., 1.f);

	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(WorldMatrix);
	m_Camera->GetViewMatrix(ViewMatrix);
	m_Direct3D->GetProjectionMatrix(ProjectionMatrix);

	RotateMatrix = DirectX::XMMatrixRotationY(Rotation);
	TranslateMatrix = DirectX::XMMatrixTranslation(-2.f, 0.f, 0.f);

	WorldMatrix = DirectX::XMMatrixMultiply(RotateMatrix, TranslateMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	Result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTexture(),
									m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!Result)
	{
		return false;
	}

	ScaleMatrix = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
	RotateMatrix = DirectX::XMMatrixRotationY(Rotation * 1.7f);
	TranslateMatrix = DirectX::XMMatrixTranslation(2.f, 0.f, 0.f);

	srMatrix = DirectX::XMMatrixMultiply(ScaleMatrix, RotateMatrix);
	WorldMatrix = DirectX::XMMatrixMultiply(srMatrix, TranslateMatrix);

	Result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!Result)
	{
		return false;
	}

	m_Direct3D->EndScene();
	
	return true;
}
