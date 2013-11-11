// $Id: curve.h,v 1.1 2005/10/19 20:08:00 weesan Exp $

/*
 * curve.h - A class for arc, but Arc is taken, thus, Curve
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/13/2005
 */

#ifndef CURVE_H
#define CURVE_H

class Curve {
private:
	double x, y, r;
	double sa, ea;

public:
	Curve(void) :
		x(0),
		y(0),
		r(0),
		sa(0),
		ea(0) {
	}
	Curve(double x, double y, double r, double sa, double ea) :
		x(x),
		y(y),
		r(r),
		sa(sa),
		ea(ea) {
	}
	double centerX(void) const {
		return (x);
	}
	double centerY(void) const {
		return (y);
	}
	double radius(void) const {
		return (r);
	}
	double startAngle(void) const {
		return (sa);
	}
	double endAngle(void) const {
		return (ea);
	}
};

#endif // CURVE_H