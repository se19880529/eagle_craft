#ifndef GLOBJECT_H
#define GLOBJECT_H
#include "Transform.h"
typedef int GLObjectID;

class GLObject
{
public:
	static GLObjectID idSeed;
	GLObject():id(idSeed++), transform(this){}
	Transform& GetTransform(){ return transform;}
	virtual void OnTransformDirty(bool onlyWorld){};
	GLObjectID GetID(){ return id; }
	virtual void Render();
	virtual void DrawGeometry();
protected:
	Transform transform;
	GLObjectID id;
};
#endif