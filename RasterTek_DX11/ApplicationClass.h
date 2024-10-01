#pragma once

#ifndef APPLICATIONCLASS_H
#define APPLICATIONCLASS_H

#include <Windows.h>

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
	bool Render();

private:

};

#endif
