#include "GLCamera.h"

const Matrix& GLCamera::GetViewMatrix()
{
	if (dirty)
	{
		dirty = false;
		worldToViewMatrix = transform.GetWorldMatrix();
		worldToViewMatrix.Invert();
	}
	return worldToViewMatrix;
}

void GLCamera::SetCurrent()
{
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	glLoadMatrixf((const float*)&GetProjectMatrix());
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	if (dirty)
	{
		dirty = false;
		worldToViewMatrix = transform.GetWorldMatrix();
		worldToViewMatrix.Invert();
	}
	glLoadMatrixf((float*)&worldToViewMatrix);
}