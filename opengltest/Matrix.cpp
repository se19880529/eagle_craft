#include "stdafx.h"
#include "Matrix.h"
#include <math.h>
Matrix::Matrix(const Matrix& m)
{
	for(int i = 0; i < MATRIX_SZ; i++)
	{
		for(int j = 0; j < MATRIX_SZ; j++)
		{
			elems[i][j] = m.elems[i][j];
		}
	}
}
Matrix::Matrix(float diag)
{
	SetDiag(diag);
}
void Matrix::Identity()
{
	SetDiag(1);
}
void Matrix::SetDiag(float e)
{
	for(int i = 0; i < MATRIX_SZ; i++)
	{
		for(int j = 0; j < MATRIX_SZ; j++)
		{
			if(i == j)
				elems[i][j] = e;
			else
				elems[i][j] = 0;
		}
	}
}
Matrix Matrix::LeftMultiply(const Matrix& matrix)
{
	Matrix res;
	for(int i = 0; i < MATRIX_SZ; i++)
		for(int j = 0; j < MATRIX_SZ; j++)
		{
			for(int k = 0; k < MATRIX_SZ; k++)
			{
				res.elems[i][j]+=elems[i][k]*matrix.elems[k][j];	
			}
		}
	return res;
}
Matrix Matrix::RightMultiply(const Matrix& matrix)
{
	return matrix.LeftMultiply(*this);
}
void Matrix::Transpose()
{
	for(int i = 0; i = MATRIX_SZ; i++)
	{
		for(int j = i+1; j < MATRIX_SZ; j++)
		{
			float temp = elems[i][j];
			elems[i][j] = elems[j][i];
			elems[j][i] = temp;
		}
	}
}
bool Matrix::Invert() 
{
	int maxi[MATRIX_SZ];
	int maxj[MATRIX_SZ];
	float elems_backup[MATRIX_SZ][MATRIX_SZ];
	memcpy(elems_backup, elems, MATRIX_SZ * MATRIX_SZ * sizeof(float));
	float max = 0;
	for(int i = 0; i < MATRIX_SZ; i++)
	{
		maxi[i] = i;
		maxj[i] = i;
		max = 0;
		for(int j = i; j < MATRIX_SZ; j++)
			for(int k = i; k < MATRIX_SZ; k++)
			{
				if(abs(elems[j][k]) > max)
				{
					max = abs(elems[j][k]);
					maxi[i] = j;
					maxj[i] = k;
				}
			}
		if(max < 0.000001f)
		{
			memcpy(elems, elems_backup, MATRIX_SZ * MATRIX_SZ * sizeof(float));
			return false;
		}
		if(maxi[i] != i)
		{
			for(int j = i; j < MATRIX_SZ; j++)
			{
				float t = elems[i][j];
				elems[i][j] = elems[maxi[i]][j];
				elems[maxi[i]][j] = t;
			}
		}
		if(maxj[i] != i)
		{
			for(int j = 0; j < MATRIX_SZ; j++)
			{
				float t = elems[j][i];
				elems[j][i] = elems[j][maxj[i]];
				elems[j][maxj[i]] = t;
			}
		}
		elems[i][i] = 1.f/max;
		for(int j = 0; j < MATRIX_SZ; j++)
		{
			if(j != i)
			{
				elems[j][i] = -elems[j][i];
				elems[i][j] *= 1.f/max;
			}
		}
		for(int j = 0; j < MATRIX_SZ; j++)
		{
			if(j != i)
			{
				for(int k = i+1; k < MATRIX_SZ; k++)
				{
					elems[j][k] += elems[i][k] * elems[j][i];
				}
			}
		}
	}
	for(int i = MATRIX_SZ-1; i >= 0; i--)
	{
		if(maxi[i] != i)
		{
			for(int j = 0; j < MATRIX_SZ; j++)
			{
				float t = elems[j][i];
				elems[j][i] = elems[j][maxi[i]];
				elems[j][maxi[i]] = t;
			}
		}
		if(maxj[i] != i)
		{
			for(int j = 0; j < MATRIX_SZ; j++)
			{
				float t = elems[i][j];
				elems[i][j] = elems[maxj[i]][j];
				elems[maxj[i]][j] = t;
			}
		}
	}
	return true;
}