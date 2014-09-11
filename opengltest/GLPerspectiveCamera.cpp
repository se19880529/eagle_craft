#include "GLPerspectiveCamera.h"
void SetCurrent()
{
	glLoadIdentity();
	if(dirty)
	{
		dirty = false;
		worldToViewMatrix = transform.GetWorldMatrix();
		worldToViewMatrix.Invert();
	}
	gl
}
