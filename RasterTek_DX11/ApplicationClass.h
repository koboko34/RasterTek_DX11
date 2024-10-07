#pragma once

#ifndef APPLICATIONCLASS_H
#define APPLICATIONCLASS_H

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightShader.h"
#include "Light.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShader* m_LightShader;
	Light* m_Lights;
	int m_NumLights;

};

#endif
