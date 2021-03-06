#include "Quaternion.h"
#include <math.h>
void Quaternion::MakeEulerRotation(float _x, float _y, float _z)
{
   float cx = cos(_x/2);
   float sx = sin(_x/2);
   float cy = cos(_y/2);
   float sy = sin(_y/2);
   float cz = cos(_z/2);
   float sz = sin(_z/2);
 
   this->w = cx*cy*cz + sx*sy*sz;
   this->x = sx*cy*cz - cx*sy*sz;
   this->y = cx*sy*cz + sx*cy*sz;
   this->z = cx*cy*sz - sx*sy*cz;
}

Matrix Quaternion::MakeRotateMatrix() const
{
	Matrix ret;
	float xx = x*x;
	float yy = y*y;
	float zz = z*z;
	float xy = x*y;
	float wz = w*z;
	float wy = w*y;
	float xz = x*z;
	float yz = y*z;
	float wx = w*x;

	ret.Element(0,0) = 1.0f-2*(yy+zz);
	ret.Element(1,0) = 2*(xy-wz);
	ret.Element(2,0) = 2*(wy+xz);
	ret.Element(3,0) = 0.0f;

	ret.Element(0,1) = 2*(xy+wz);
	ret.Element(1,1) = 1.0f-2*(xx+zz);
	ret.Element(2,1) = 2*(yz-wx);
	ret.Element(3,1) = 0.0f;

	ret.Element(0,2) = 2*(xy-wy);
	ret.Element(1,2) = 2*(yz+wx);
	ret.Element(2,2) = 1.0f-2*(xx+yy);
	ret.Element(3,2) = 0.0f;

	ret.Element(0,3) = 0.0f;
	ret.Element(1,3) = 0.0f;
	ret.Element(2,3) = 0.0f;
	ret.Element(3,3) = 1.0f;

	return ret;
}
