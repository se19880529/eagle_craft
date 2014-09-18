#ifndef RENDERTARGETWINDOWS_H
#define RENDERTARGETWINDOWS_H
#include "GLRenderTarget.h"
#include <windows.h>
class RenderTargetWindows : public RenderTarget
{
public:
	RenderTargetWindows(HWND hWnd);
	void Init(){}
	void SetCurrent();
private:
	HGLRC hRC;
	HWND mhWnd;
	HDC hDC;
	int invalidCode;
};
#endif