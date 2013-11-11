// $Id: Coord.cpp,v 1.4 2005/12/03 19:10:32 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#include "stdafx.h"
#include "Coord.h"

#include <math.h>

ostream& operator<< (ostream& output, const Coord& v)
{
	output<< v.x << ' ' << v.y;
	return output;
}

Coord::Coord(double xx, double yy) : Object("Coord"), x(xx), y(yy) {
}

bool Coord::operator== (const Coord& c) const
{
	return(fabs(x-c.x)<COORD_EPSILON && fabs(y-c.y)<COORD_EPSILON);
}
bool Coord::operator!= (const Coord& c) const
{
	return(!((*this)==c));
}
const Coord Coord::operator+ (const Coord& c) const
{
	return(Coord(x+c.x,y+c.y));
}
const Coord Coord::operator- (const Coord& c) const
{
	return(Coord(x-c.x,y-c.y));
}

// Added by weesan@cs.ucr.edu
bool Coord::operator<(const Coord &c) const {
	if (x < c.x) {
		return (true);
	} else if (x == c.x) {
		return (y < c.y);
	} else {
		return (false);
	}
}

void Coord::set(double xx,double yy)
{
	x=xx;
	y=yy;
}

double Coord::distanceTo(const Coord &c) const
{
	double dx,dy;
	dx = x - c.x;
	dy = y - c.y;
	return sqrt(dx*dx + dy*dy);
}
void Coord::Translate(double dx, double dy)
{
	x += dx;
	y += dy;
}
void Coord::Rotate(double theta)
{
	static double oldx,oldy;
	oldx = x;
	oldy = y;
	x = oldx*cos(theta) - oldy*sin(theta);
	y = oldx*sin(theta) + oldy*cos(theta);
}
void Coord::Scale(double sx, double sy)
{
	x *= sx;
	y *= sy;
}
