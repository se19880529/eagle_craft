#ifndef GLRENDERMANAGER_H
#define GLRENDERMANAGER_H
#include "GLCamera.h"
#include "Transform.h"
#include "GLRenderTarget.h"
#include "GLObject.h"
#include <map>
class GLRenderManager
{
public:
	GLRenderManager(){};
	void Render();
	void AddRenderObject(GLObject* obj);
	void DelRenderObject(GLObject* obj);
	void AddCamera(GLCamera* cam);
	void DelCamera(GLCamera* cam);
protected:
	std::map<GLObjectID, GLCamera*> cameras;
	std::map<GLObjectID, GLObject*> objects;
};
#endif