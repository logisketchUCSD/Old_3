// $Id: line.h,v 1.2 2006/11/03 07:14:43 weesan Exp $

/*
 * line.h - a class for line
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/13/2005
 */

#ifndef LINE_H
#define LINE_H

#include <math.h>

class Line {
private:
	double _x1, _y1, _x2, _y2;

public:
	Line(void) :
		_x1(0),
		_y1(0),
		_x2(0),
		_y2(0) {
	}
	Line(double x1, double y1, double x2, double y2) :
		_x1(x1),
		_y1(y1),
		_x2(x2),
		_y2(y2) {
	}
	double x1(void) const {
		return (_x1);
	}
	double y1(void) const {
		return (_y1);
	}
	double x2(void) const {
		return (_x2);
	}
	double y2(void) const {
		return (_y2);
	}
	double length(void) const {
		int x = x2() - x1();
		int y = y2() - y1();
		return (sqrt((float)(x * x + y * y)));
	}
	double gradient(void) const {
		return ((y2() - y1()) / (x2() - x1()));
	}
};

#endif 