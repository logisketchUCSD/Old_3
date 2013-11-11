// $Id: matrix.cpp,v 1.1 2005/11/04 22:29:16 weesan Exp $

/*
 * matrix.cpp - A matrix implementation
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/19/2005
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "matrix.h"

Matrix::Matrix(int iRow, int iCol) :
	iRow(iRow),
	iCol(iCol) {
	// Expand the row
	resize(iRow);
	// Then, for each row, expand its col
	for (int i = 0; i < size(); i++) {
		(*this)[i].resize(iCol);
	}
}

void Matrix::data(int iHowMany, ...) {
	va_list ap;
	va_start(ap, iHowMany);

	for (int i = 0; i < iHowMany; i++) {
		double dData = va_arg(ap, double);
		(*this)[i / col()][i % col()] = dData;
	}

	va_end(ap);
}


void Matrix::dump(void) {
	for (int i = 0; i < size(); i++) {
		for (int j = 0; j < (*this)[i].size(); j++) {
			printf("%.1f ", (*this)[i][j]);
		}
		printf("\n");
	}
}
