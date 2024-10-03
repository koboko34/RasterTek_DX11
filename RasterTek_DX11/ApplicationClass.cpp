#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass&)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialise(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	m_Direct3D = new D3DClass();

	bool Result = m_Direct3D->Initialise(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!Result)
	{
		ShowCursor(true);
		MessageBox(hwnd, L"Could not initialise Direct3D!", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{
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
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5, 1.f);

	m_Direct3D->EndScene();
	
	return true;
}
