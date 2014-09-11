#ifndef GLCAMERA_H
#define GLCAMERA_H
#include "GLBase.h"
class GLCamera : GLObject
{
public:
	GLCamera():far(-1),near(0){}
	virtual void SetCurrent() = 0;
	void SetFar(float f){ far = f;}
	void SetNear(float n){ near = n;}
	float GetFar(){ return far;}
	float GetNear(){ return near;}
protected:
	float far;
	float near;
};
#endif
