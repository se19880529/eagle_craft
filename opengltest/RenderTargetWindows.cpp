#include "RenderTargetWindows.h"

RenderTargetWindows::RenderTargetWindows(HWND hWnd)
{
	invalidCode = 0;
	mhWnd = hWnd;
	hDC = GetDC(hWnd);
	static PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (pixelFormat != 0)
		invalidCode = 1;			//pixelFormat cannot found
	else
	{
		invalidCode = 0;
		SetPixelFormat(hDC, pixelFormat, &pfd);
		hRC = wglCreateContext(hDC);
	}
}

void RenderTargetWindows::SetCurrent()
{
	if (invalidCode == 0)
		wglMakeCurrent(hDC, hRC);
}