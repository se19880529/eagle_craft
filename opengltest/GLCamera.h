#ifndef GLCAMERA_H
#define GLCAMERA_H
#include "GLTBase.h"
#include "GLObject.h"
class GLCamera : public GLObject
{
public:
	GLCamera():dirty(true){}
	void SetCurrent();
	virtual const Matrix& GetProjectMatrix() = 0;
	void OnTransformDirty(bool onlyWorld){ dirty = true; }
	const Matrix& GetViewMatrix();
protected:
	bool dirty;
	Matrix worldToViewMatrix;
	
};
#endif