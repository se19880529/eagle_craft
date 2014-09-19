#ifndef GLPERSPECTIVECAMERA_H
#define GLPERSPECTIVECAMERA_H
#include "GLCamera.h"
class GLPerspectiveCamera : public GLCamera
{
public:
	GLPerspectiveCamera(){}
	const Matrix& GetProjectMatrix();
	void SetFOV(float f){ fov = f; dirty = true; }
	float GetFOV(){ return fov; }
	void SetFar(float f){ farClipPlane = f; dirty = true; }
	void SetNear(float n){ nearClipPlane = n; dirty = true; }
	float GetFar(){ return farClipPlane; }
	float GetNear(){ return nearClipPlane; }
protected:
	float fov;
	float farClipPlane;
	float nearClipPlane;
	bool dirty;
	Matrix matrixProject;
};
#endif