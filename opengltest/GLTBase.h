#include <math.h>
typedef float velem_t;
class ELVector 
{
public:
	velem_t x;
	velem_t y;
	velem_t z;
	ELVector(velem_t _x, velem_t _y, velem_t _z) : x(_x), y(_y), z(_z){}
	ELVector(const ELVector& v) : x(v.x), y(v.y), z(v.z){}
	ELVector(velem_t e) : x(e), y(e), z(e){}
	ELVector():x(0),y(0),z(0){}
	velem_t DotProduct(const ELVector& v)
	{
		return x*v.x+y*v.y+z*v.z;
	}
	velem_t Norm()
	{
		return x*x+y*y+z*z;
	}
	velem_t Length()
	{
		return sqrt(Norm());
	}
	void Normalize()
	{
		velem_t len = Length();
		x/=len;
		y/=len;
		z/=len;
	}
	ELVector& operator=(const ELVector& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	ELVector operator+(const ELVector& v)
	{
		ELVector v(x+v.x, y+v.y, z+v.z);
		return v;
	}
	ELVector operator-()
	{
		ELVector v(-x, -y, -z);
		return v;
	}
	ELVector operator-(const ELVector& v)
	{
		ELVector v(x-v.x, y-v.y, z-v.z);
		return v;
	}
	ELVector operator*(velem_t s)
	{
		ELVector v(x*s, y*s, z*s);
		return v;
	}
	ELVector operator*(const ELVector& v)
	{
		ELVector v(y*v.z-v.y*z, z*v.x-v.z*x, x*v.y-v.x*y);
		return v;
	}
};

class ELVector4 
{
public:
	velem_t x;
	velem_t y;
	velem_t z;
	velem_t w;
	ELVector4(velem_t _x, velem_t _y, velem_t _z, velem_t _w) : x(_x), y(_y), z(_z), w(_w){}
	ELVector4(const ELVector& v) : x(v.x), y(v.y), z(v.z), w(v.w){}
	ELVector4(velem_t e) : x(e), y(e), z(e), w(e){}
	ELVector4():x(0),y(0),z(0), w(0){}
	velem_t DotProduct(const ELVector& v)
	{
		return x*v.x+y*v.y+z*v.z+w*v.w;
	}
	velem_t Norm()
	{
		return x*x+y*y+z*z+w*w;
	}
	velem_t Length()
	{
		return sqrt(Norm());
	}
	void Normalize()
	{
		velem_t len = Length();
		x/=len;
		y/=len;
		z/=len;
		w/=len;
	}
	ELVector4& operator=(const ELVector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = w.z;
		return *this;
	}
	ELVector4 operator+(const ELVector4& v)
	{
		ELVector4 v(x+v.x, y+v.y, z+v.z, w+v.w);
		return v;
	}
	ELVector4 operator-()
	{
		ELVector4 v(-x, -y, -z, -w);
		return v;
	}
	ELVector4 operator-(const ELVector4& v)
	{
		ELVector4 v(x-v.x, y-v.y, z-v.z, w-v.w);
		return v;
	}
	ELVector4 operator*(velem_t s)
	{
		ELVector4 v(x*s, y*s, z*s, w*s);
		return v;
	}
};

template<int SZ>
class Matrix
{
public:
	velem_t elems[SZ][SZ];			
	Matrix(const Matrix<SZ>& m)
	{
		for(int i = 0; i < SZ; i++)
		{
			for(int j = 0; j < SZ; j++)
			{
				elems[i][j] = m.elems[i][j];
			}
		}
	}
	Matrix()
	{
		SetDiag(0);
	}
	void Identity()
	{
		SetDiag(1);
	}
	void SetDiag(velem_t e)
	{
		for(int i = 0; i < SZ; i++)
		{
			for(int j = 0; j < SZ; j++)
			{
				if(i == j)
					elems[i][j] = e;
				else
					elems[i][j] = 0;
			}
		}
	}
	Matrix<SZ> LeftMultiply(const Matrix<SZ>& matrix)
	{
		Matrix<SZ> res;
		for(int i = 0; i < SZ; i++)
			for(int j = 0; j < SZ; j++)
			{
				for(int k = 0; k < SZ; k++)
				{
					res.elems[i][j]+=elems[i][k]*matrix.elems[k][j];	
				}
			}
		return res;
	}
	Matrix<SZ> RightMultiply(const Matrix<SZ>& matrix)
	{
		return matrix.LeftMultiply(*this);
	}
	Matrix<SZ> Invert()
	{
		int maxi[SZ];
		int maxj[SZ];
		velem_t max = 0;
		for(int i = 0; i < SZ; i++)
		{
			maxi[i] = i;
			maxj[i] = j;
			
		}
		return Matrix<SZ>();
	}
};
typedef Matrix<4> Matrix4x4;
typedef Matrix<3> Matrix3x3;
