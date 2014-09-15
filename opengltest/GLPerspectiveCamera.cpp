#include "GLPerspectiveCamera.h"
void SetCurrent()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(dirty)
	{
		dirty = false;
		worldToViewMatrix = transform.GetWorldMatrix();
		worldToViewMatrix.Invert();
	}
	glLoadMatrixf((float*)&worldToViewMatrix);
}
