#ifndef MATRIX_H
#define MATRIX_H
#define MATRIX_SZ 4
class Matrix
{
public:
	float elems[MATRIX_SZ][MATRIX_SZ];			
	Matrix(const Matrix& m);
	Matrix(float diag = 0);
	void Identity();
	void SetDiag(float e);
	Matrix LeftMultiply(const Matrix& matrix);
	Matrix RightMultiply(const Matrix& matrix);
	bool Invert();
	void Transpose();
};
#endif