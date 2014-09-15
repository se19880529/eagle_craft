#ifndef GLPERSPECTIVECAMERA_H
#define GLPERSPECTIVECAMERA_H
#include "GLCamera.h"
class GLPerspectiveCamera
{
public:
	GLPerspectiveCamera(){}
	void OnTransformDirty(bool onlyWorld){ dirty = true;}
	void SetCurrent();
	void SetFOV(float f){ fov = f; dirty = true; }
	float GetFOV(){ return fov; }
protected:
	Matrix worldToViewMatrix;
	float fov;
};
#endif
