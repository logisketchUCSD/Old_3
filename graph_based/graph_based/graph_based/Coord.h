// $Id: Coord.h,v 1.3 2005/07/26 18:19:04 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


//A storage class for x-y coordinates (in doubles). 
//Provides some useful functionality like rotation etc.
//x-y coordinates declared 'public' for easy access and manipulation
#ifndef COORD_H
#define COORD_H
//created by Levent Burak Kara

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Added by weesan@cs.ucr.edu
#include "object.h"

#define COORD_EPSILON 10e-6


class Coord : public Object {   
	friend std::ostream& operator<< (std::ostream&, const Coord&);
public:
	double x;
	double y;

public:
	Coord(double xx = 0.0, double yy = 0.0);
	bool operator== (const Coord& c) const;
	bool operator!= (const Coord& c) const;
	const Coord operator+ (const Coord& c) const;
	const Coord operator- (const Coord& c) const;
	// Added by weesan@cs.ucr.edu
	bool operator<(const Coord &c) const;

	void set(double xx,double yy);

	double distanceTo(const Coord &c) const;
	void Translate(double dx, double dy);
	void Rotate(double theta);
	void Scale(double sx, double sy);

protected:
	void write(std::ostream &os) const {
		os << x << ' ' << y;
	}
	void read(std::istream &is) {
		is >> x >> y;
	}
};


#endif //COORD_H