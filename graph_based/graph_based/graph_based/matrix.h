// $Id: matrix.h,v 1.1 2005/11/04 22:29:16 weesan Exp $

/*
 * matrix.h - A matrix implementation
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/19/2005
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <math.h>
#include <assert.h>

#define MATRIX_DIMENSION_DEFAULT	3

using namespace std;

class Matrix : public vector<vector<double> > {
private:
	int iRow, iCol;

public:
	Matrix(int iRow = MATRIX_DIMENSION_DEFAULT,
		   int iCol = MATRIX_DIMENSION_DEFAULT);
	int row(void) const {
		return (iRow);
	}
	int col(void) const {
		return (iCol);
	}
	double x(void) const {
		// Assuming column vector, good for (x,y) being used as 3x1 matrix
		return ((*this)[0][0]);
	}
	double y(void) const {
		// Assuming column vector, good for (x,y) being used as 3x1 matrix
		return ((*this)[1][0]);
	}
	void data(int iHowMany, ...);
	void dump(void);
};

class MatrixTranslation : public Matrix {
public:
	MatrixTranslation(double x, double y) : Matrix(3, 3) {
		data(9,
			 1.0, 0.0, x,
			 0.0, 1.0, y,
			 0.0, 0.0, 1.0);
	}
};

class MatrixRotation : public Matrix {
public:
	MatrixRotation(double a) : Matrix(3, 3) {
		data(9,
			 cos(a), -sin(a), 0.0,
			 sin(a), cos(a),  0.0,
			 0.0,	 0.0,	  1.0);
	}
};

class MatrixScaling : public Matrix {
public:
	MatrixScaling(double x, double y) : Matrix(3, 3) {
		data(9,
			 x,	  0.0, 0.0,
			 0.0, y,   0.0,
			 0.0, 0.0, 1.0);
	}
};

inline Matrix operator*(Matrix &a, Matrix &b) {
	assert(a.col() == b.row());

	Matrix c(a.row(), b.col());
	for (int i = 0; i < a.row(); i++) {
		for (int j = 0; j < b.col(); j++) {
			for (int k = 0; k < b.row(); k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return (c);
}

#endif // MATRIX_H