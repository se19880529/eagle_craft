#inlcude <math.h>
#include "GLPerspectiveCamera.h"
const Matrix& GLPerspectiveCamera::GetProjectMatrix()
{
	if (dirty)
	{
		float tgent = tan(fov / 2);
		float ratio = 16.f / 9.f;
		matrixProject.Identity();
		matrixProject.Element(0, 0) = 1.f / (tgent*ratio);
		matrixProject.Element(1, 1) = 1.f / tgent;
		matrixProject.Element(2, 2) = farClipPlane / (nearClipPlane - farClipPlane);
		matrixProject.Element(2, 3) = matrixProject.Element(2, 2) * nearClipPlane;
		matrixProject.Element(3, 2) = -1;
		matrixProject.Element(3, 3) = 0;
		dirty = false;
	}
	return matrixProject;
}