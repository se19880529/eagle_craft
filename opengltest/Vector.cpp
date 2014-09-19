#include "Vector.h"
#include <math.h>
float ELVector2::DotProduct(const ELVector2& v)
{
	return x*v.x + y*v.y;
}
float ELVector2::Norm()
{
	return x*x + y*y ;
}
float ELVector2::Length()
{
	return sqrt(Norm());
}
void ELVector2::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
}
ELVector2& ELVector2::operator=(const ELVector2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}
ELVector2 ELVector2::operator+(const ELVector2& v)
{
	ELVector2 res(x + v.x, y + v.y);
	return res;
}
ELVector2 ELVector2::operator-()
{
	ELVector2 v(-x, -y);
	return v;
}
ELVector2 ELVector2::operator-(const ELVector2& v)
{
	ELVector2 res(x - v.x, y - v.y);
	return res;
}
ELVector2 ELVector2::operator*(float s)
{
	ELVector2 v(x*s, y*s);
	return v;
}


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
	ELVector res(x+v.x, y+v.y, z+v.z);
	return res;
}
ELVector ELVector::operator-()
{
	ELVector v(-x, -y, -z);
	return v;
}
ELVector ELVector::operator-(const ELVector& v)
{
	ELVector res(x-v.x, y-v.y, z-v.z);
	return res;
}
ELVector ELVector::operator*(float s)
{
	ELVector v(x*s, y*s, z*s);
	return v;
}
ELVector ELVector::operator*(const ELVector& v)
{
	ELVector res(y*v.z-v.y*z, z*v.x-v.z*x, x*v.y-v.x*y);
	return res;
}


float ELVector4::DotProduct(const ELVector4& v)
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
	w = v.z;
	return *this;
}
ELVector4 ELVector4::operator+(const ELVector4& v)
{
	ELVector4 res(x+v.x, y+v.y, z+v.z, w+v.w);
	return res;
}
ELVector4 ELVector4::operator-()
{
	ELVector4 res(-x, -y, -z, -w);
	return res;
}
ELVector4 ELVector4::operator-(const ELVector4& v)
{
	ELVector4 re(x-v.x, y-v.y, z-v.z, w-v.w);
	return re;
}
ELVector4 ELVector4::operator*(float s)
{
	ELVector4 v(x*s, y*s, z*s, w*s);
	return v;
}