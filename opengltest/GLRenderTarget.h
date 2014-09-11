#ifndef GLRENDERTARGET_H
#define GLRENDERTARGET_H
class RenderTarget
{
public:
	virtual void Init() = 0;
	virtual void SetCurrent() = 0;
};
#endif
