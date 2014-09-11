#include "Transform.h"

class GLObject
{
public:
	Transform& GetTransform(){ return transform;}
protected:
	Transform transform;
};