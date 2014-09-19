#include "GLObject.h"

void GLObject::DrawGeometry(){}
void GLObject::Render()
{
	glPushMatrix();
	glMultMatrixf((const float*)&transform.GetWorldMatrix());
	DrawGeometry();
	glPopMatrix();
}