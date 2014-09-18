#include "Vector.h"
#include <math.h>
float ELVector::DotProduct(const ELVector& v)
{
	return x*v.x+y*v.y+z*v.z;
}
float ELVector::Norm()
{
	return x*x+y*y+z*z;
}
float ELVector::Length()
{
	return sqrt(Norm());
}
void ELVector::Normalize()
{
	float len = Length();
	x/=len;
	y/=len;
	z/=len;
}
ELVector& ELVector::operator=(const ELVector& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}
ELVector ELVector::operator+(const ELVector& v)
{
	ELVector v(x+v.x, y+v.y, z+v.z);
	return v;
}
ELVector ELVector::operator-()
{
	ELVector v(-x, -y, -z);
	return v;
}
ELVector ELVector::operator-(const ELVector& v)
{
	ELVector v(x-v.x, y-v.y, z-v.z);
	return v;
}
ELVector ELVector::operator*(float s)
{
	ELVector v(x*s, y*s, z*s);
	return v;
}
ELVector ELVector::operator*(const ELVector& v)
{
	ELVector v(y*v.z-v.y*z, z*v.x-v.z*x, x*v.y-v.x*y);
	return v;
}
float ELVector4::DotProduct(const ELVector& v)
{
	return x*v.x+y*v.y+z*v.z+w*v.w;
}
float ELVector4::Norm()
{
	return x*x+y*y+z*z+w*w;
}
float ELVector4::Length()
{
	return sqrt(Norm());
}
void ELVector4::Normalize()
{
	float len = Length();
	x/=len;
	y/=len;
	z/=len;
	w/=len;
}
ELVector4& ELVector4::operator=(const ELVector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = w.z;
	return *this;
}
ELVector4 ELVector4::operator+(const ELVector4& v)
{
	ELVector4 v(x+v.x, y+v.y, z+v.z, w+v.w);
	return v;
}
ELVector4 ELVector4::operator-()
{
	ELVector4 v(-x, -y, -z, -w);
	return v;
}
ELVector4 ELVector4::operator-(const ELVector4& v)
{
	ELVector4 v(x-v.x, y-v.y, z-v.z, w-v.w);
	return v;
}
ELVector4 ELVector4::operator*(float s)
{
	ELVector4 v(x*s, y*s, z*s, w*s);
	return v;
}