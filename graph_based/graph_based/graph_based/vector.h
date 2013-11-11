// $Id: vector.h,v 1.1 2005/10/19 20:08:01 weesan Exp $

/*
 * vector.h - A class to implement vector as in linear algebra
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/13/2005
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vector {
private:
	double _x, _y;

public:
	Vector(void) :
		_x(0),
		_y(0) {
	}
	Vector(double x, double y) :
		_x(x),
		_y(y) {
	}
	double x(void) const {
		return (_x);
	}
	double y(void) const {
		return (_y);
	}
	double length(void) const {
		return (sqrt(x() * x() + y() * y()));
	}
	Vector &operator+=(Vector &a) {
		_x += a.x();
		_y += a.y();
		return (*this);
	}

	void dump(void) {
		printf("(%.02f, %.02f)\n", x(), y());
	}
};

inline Vector operator+=(Vector &a, double dScale) {
	return (Vector(a.x() + dScale, a.y() + dScale));
}

inline Vector operator-(Vector &a, Vector &b) {
	return (Vector(a.x() - b.x(), a.y() - b.y()));
}

inline Vector operator*(Vector &a, double dScale) {
	return (Vector(a.x() * dScale, a.y() * dScale));
}

inline Vector operator/(Vector &a, double dScale) {
	return (Vector(a.x() / dScale, a.y() / dScale));
}

inline bool operator>(Vector &a, Vector &b) {
	return (a.length() > b.length());
}

inline bool operator<=(Vector &a, Vector &b) {
	return (a.length() <= b.length());
}

inline bool operator!=(Vector &a, Vector &b) {
	return (a.x() != b.x() || a.y() != b.y());
}

inline void swapVector(Vector &a, Vector &b) {
	Vector t = a;
	a = b;
	b = t;
}

inline Vector unitVector(Vector &a, Vector &b) {
	Vector c = b - a;
	return (c / c.length());
}

#endif // VECTOR_H