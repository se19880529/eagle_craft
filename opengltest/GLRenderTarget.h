#ifndef GLRENDERTARGET_H
#define GLRENDERTARGET_H
class RenderTarget
{
public:
	virtual void Init() = 0;
	virtual void SetCurrent() = 0;
	virtual void BeforeRender() = 0;
	virtual void AfterRender() = 0;
};
#endif