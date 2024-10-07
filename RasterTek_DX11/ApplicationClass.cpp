#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Lights = 0;
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
	m_Camera->SetPosition(0.f, 2.f, -12.f);

	strcpy_s(ModelFilename, "Models/plane.txt");
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

	m_NumLights = 4;

	m_Lights = new Light[m_NumLights];

	m_Lights[0].SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
	m_Lights[0].SetPosition(-3.f, 1.f, 3.f);

	m_Lights[1].SetDiffuseColor(0.f, 1.f, 0.f, 1.f);
	m_Lights[1].SetPosition(3.f, 1.f, 3.f);

	m_Lights[2].SetDiffuseColor(0.f, 0.f, 1.f, 1.f);
	m_Lights[2].SetPosition(-3.f, 1.f, -3.f);

	m_Lights[3].SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	m_Lights[3].SetPosition(3.f, 1.f, -3.f);

	return true;
}

void ApplicationClass::Shutdown()
{
	if (m_Lights)
	{
		delete[] m_Lights;
		m_Lights = 0;
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
	DirectX::XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix;
	DirectX::XMFLOAT4 DiffuseColors[4], LightPositions[4];
	bool Result;
	
	m_Direct3D->BeginScene(0.f, 0.f, 0., 1.f);

	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(WorldMatrix);
	m_Camera->GetViewMatrix(ViewMatrix);
	m_Direct3D->GetProjectionMatrix(ProjectionMatrix);

	for (int i = 0; i < m_NumLights; i++)
	{
		DiffuseColors[i] = m_Lights[i].GetDiffuseColor();
		LightPositions[i] = m_Lights[i].GetPosition();
	}

	m_Model->Render(m_Direct3D->GetDeviceContext());

	Result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTexture(), DiffuseColors, LightPositions);
	if (!Result)
	{
		return false;
	}

	m_Direct3D->EndScene();
	
	return true;
}
