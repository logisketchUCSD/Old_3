// $Id: confusion_matrix.h,v 1.1 2006/05/19 00:46:08 weesan Exp $

/*
 * confusion_matrix.h - A confusion matrix implementation.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/16/2006
 */

#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

#include <map>
#include <string>

using namespace std;

typedef map<string, int, less<string> > confusion_matrix_col_t;
typedef map<string, confusion_matrix_col_t, less<string> > confusion_matrix_row_t;

class ConfusionMatrix : public confusion_matrix_row_t {
public:
	void dump(FILE *fp);
};

#endif // CONFUSION_MATRIX_H