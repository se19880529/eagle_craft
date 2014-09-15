#ifndef MATRIX_H
#define MATRIX_H
#define MATRIX_SZ 4
class Matrix
{
public:
	Matrix(const Matrix& m);
	Matrix(float diag = 0);
	void Identity();
	void SetDiag(float e);
	float& Element(int row, int col)
	{
		if(row >= MATRIX_SZ)
			row = row % MATRIX_SZ;
		if(col >= MATRIX_SZ)
			col = col % MATRIX_SZ;
		return elems[row][col];
	}
	Matrix LeftMultiply(const Matrix& matrix);
	Matrix RightMultiply(const Matrix& matrix);
	bool Invert();
	void Transpose();
protected:
	float elems[MATRIX_SZ][MATRIX_SZ];			//stored by row-major order
};
#endif
