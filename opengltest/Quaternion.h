#include "Matrix.h"
#ifndef QUATERNION_H
#define QUATERNION_H
class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;
	Quaternion(float _x = 0, float _y = 0, float _z = 0, float _w = 0) : x(_x), y(_y), z(_z), w(_w){}
	void MakeEulerRotation(float x, float y, float z);
	Matrix MakeRotateMatrix() const;
}
#endif