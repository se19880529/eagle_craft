#ifndef VECTOR_H
#define VECTOR_H
class ELVector 
{
public:
	float x;
	float y;
	float z;
	ELVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
	ELVector(const ELVector& v) : x(v.x), y(v.y), z(v.z){}
	ELVector(float e) : x(e), y(e), z(e){}
	ELVector():x(0),y(0),z(0){}
	float DotProduct(const ELVector& v);
	float Norm();
	float Length();
	void Normalize();
	ELVector& operator=(const ELVector& v);
	ELVector operator+(const ELVector& v);
	ELVector operator-();
	ELVector operator-(const ELVector& v);
	ELVector operator*(float s);
	ELVector operator*(const ELVector& v);
};
class ELVector4 
{
public:
	float x;
	float y;
	float z;
	float w;
	ELVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
	ELVector4(const ELVector& v) : x(v.x), y(v.y), z(v.z), w(v.w){}
	ELVector4(float e) : x(e), y(e), z(e), w(e){}
	ELVector4():x(0),y(0),z(0), w(0){}
	float DotProduct(const ELVector& v);
	float Norm();
	float Length();
	void Normalize();
	ELVector4& operator=(const ELVector4& v);
	ELVector4 operator+(const ELVector4& v);
	ELVector4 operator-();
	ELVector4 operator-(const ELVector4& v);
	ELVector4 operator*(float s);
};
#endif