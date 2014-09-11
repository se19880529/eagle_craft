#include "Transform.h"

class GLObject
{
public:
	GLObject():transform(this){}
	Transform& GetTransform(){ return transform;}
	virtual void OnTransformDirty(bool onlyWorld){};
protected:
	Transform transform;
};
