// $Id: algebra.cpp,v 1.7 2005/12/16 00:00:54 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#include <math.h>
#include "stdafx.h"
#include "algebra.h"


void swap(double& a,double& b)
{
	double t=a;	a=b; b=t;
}
bool equal(double a,double b)
{
	return fabs(a-b)<EQUALITY_EPS;
}
double sq(double a)
{
	return a*a;
}

double dot(double ax,double ay,double bx,double by)
{
	return (ax*bx+ay*by);
}

double cross(double ax,double ay,double bx,double by)
{
	return (ax*by-bx*ay);
}

// Added by weesan@cs.ucr.edu to compute Euclidean's distance
double distance(double x1, double y1, double x2, double y2) {
	double xDiff = x1 - x2;
	double yDiff = y1 - y2;
	return (sqrt(xDiff * xDiff + yDiff * yDiff));
}

double Deg2Rad(double d)
{
	return d*PI/180.0;
}
double Rad2Deg(double r)
{
	return r*180.0/PI;
}

double mean(double sumX,int N)
{
	assert(N>0);
	return (sumX/(double)N);
}
double std_dev(double sumX,double sumXsqr,int N)
{
	assert(N>1);
	return sqrt((double(N) * sumXsqr - sumX * sumX) /  (double(N) * double(N - 1)));
}

double Angle(Coord sA,Coord eA,Coord sB,Coord eB)
{
	assert(!equal(sA.distanceTo(eA),0.0) && !equal(sB.distanceTo(eB),0.0));

	static double Ax,Ay,Bx,By;
	Ax=eA.x-sA.x;
	Ay=eA.y-sA.y;
	Bx=eB.x-sB.x;
	By=eB.y-sB.y;

	//acos returns between 0 and PI
	return acos((Ax*Bx+Ay*By)/sA.distanceTo(eA)/sB.distanceTo(eB));
}
double Acute_Angle(Coord sA,Coord eA,Coord sB,Coord eB)
{
	double angle= Angle(sA,eA,sB,eB);
	if(angle > (PI/2.0)) angle=PI-angle;
	assert(angle>=0.0 && angle<=(PI/2.0));
	return angle;
}
bool Parallel(Coord sA,Coord eA,Coord sB,Coord eB)
{
	return ( fabs(Acute_Angle(sA,eA,sB,eB)) < PARALLELISM_ANGLE_ZONE);
}
bool Perpendicular(Coord sA,Coord eA,Coord sB,Coord eB)
{
	return ( fabs(Acute_Angle(sA,eA,sB,eB) - PI/2.0) < PARALLELISM_ANGLE_ZONE);
}

//http://astronomy.swin.edu.au/~pbourke/geometry/lineline2d/
int  LineSegLineSeg_intersect(Coord sA,Coord eA,Coord sB,Coord eB, Coord& intP)
{
	double x1,x2,x3,x4,y1,y2,y3,y4,u,t,det;

	x1=sA.x;	y1=sA.y;
	x2=eA.x;	y2=eA.y;
	x3=sB.x;	y3=sB.y;
	x4=eB.x;	y4=eB.y;

	det=(y4-y3)*(x2-x1)-(x4-x3)*(y2-y1);

	if(equal(0.0 , det)) return 0;

	t=((x4-x3)*(y1-y3)-(y4-y3)*(x1-x3))/det;
	u=((x2-x1)*(y1-y3)-(y2-y1)*(x1-x3))/det;

	if(t>=0.0-INTERSECTION_TOLERANCE_PERCENTAGE && 
	   t<=1.0+INTERSECTION_TOLERANCE_PERCENTAGE && 
	   u>=0.0-INTERSECTION_TOLERANCE_PERCENTAGE &&
	   u<=1.0+INTERSECTION_TOLERANCE_PERCENTAGE)
	{
		intP.x=x1+t*(x2-x1);
		intP.y=y1+t*(y2-y1);
		return 1;
	}
	
	return 0;
}

//http://astronomy.swin.edu.au/~pbourke/geometry/sphereline/
int LineSegCircle_intersect(Coord sL,Coord eL,Coord cen,double r, Coord& intP1, Coord& intP2)
{
	double x1,x2,x3,y1,y2,y3,a,b,c,det,u;
	int numof_int = 0;

	x1=sL.x;	y1=sL.y;
	x2=eL.x;	y2=eL.y;
	x3=cen.x;	y3=cen.y;

	a = sq(x2 - x1) + sq(y2 - y1);
	b = 2*((x2 - x1)*(x1 - x3) + (y2 - y1)*(y1 - y3));
	c = sq(x3) + sq(y3) + sq(x1) + sq(y1) - 2*(x3*x1 + y3*y1) - sq(r);

	if(equal(a,0.0)) return 0;

	det=sq(b)-4*a*c;

	if(equal(det,0.0))  //one intersection
	{
		u=(-b)/2.0/a;
		if(u>(1+INTERSECTION_TOLERANCE_PERCENTAGE) || u<(0-INTERSECTION_TOLERANCE_PERCENTAGE)) return 0;
		intP1.x=x1+u*(x2-x1);
		intP1.y=y1+u*(y2-y1);
		return 1;
	}
	
	if(det < 0.0) //no intersection
	{
		return 0;
	}
	else
	{
		u= (-b-sqrt(det))/2.0/a;
		if( ! (u>(1+INTERSECTION_TOLERANCE_PERCENTAGE) || u<(0-INTERSECTION_TOLERANCE_PERCENTAGE)))
		{
			intP1.x=x1+u*(x2-x1);
			intP1.y=y1+u*(y2-y1);
			numof_int++;
		}

		u= (-b+sqrt(det))/2.0/a;
		if( ! (u>(1+INTERSECTION_TOLERANCE_PERCENTAGE) || u<(0-INTERSECTION_TOLERANCE_PERCENTAGE)))
		{
			intP2.x=x1+u*(x2-x1);
			intP2.y=y1+u*(y2-y1);
			numof_int++;
			if(numof_int==1) intP1=intP2;
		}
		return numof_int;
	}

		

	return 0;
}

//http://www.magic-software.com/Intersection2D.html
int CircleCircle_intersect(Coord cenA,double rA,Coord cenB,double rB, Coord& intP1, Coord& intP2)
{
	static double s,t;
	Coord U(cenB-cenA);
	Coord V(U.y,-U.x);
	static Coord origin(0.0,0.0);

	double Umag=U.distanceTo(origin);

	if(equal(Umag,0.0)) return 0;

	if(equal(rA+rB,Umag))  //one intersection
	{
		intP1.x=cenA.x+(rA/(rA+rB))*U.x;
		intP1.y=cenA.y+(rA/(rA+rB))*U.y;
		return 1;
	}

	if((rA+rB)<Umag)
		return 0;

	if(fabs(rA-rB)>Umag)
		return 0;

	else //there are 2 intersections
	{
		s=0.50*((sq(rA)-sq(rB))/sq(Umag)+1.0);

		t=sqrt(sq(rA)/sq(Umag)-sq(s));
		
		intP1.x=cenA.x+s*U.x+t*V.x;
		intP1.y=cenA.y+s*U.y+t*V.y;

		t=-t;
		intP2.x=cenA.x+s*U.x+t*V.x;
		intP2.y=cenA.y+s*U.y+t*V.y;
		return 2;
	}
	return 0;
}

/*
 * Determine if a point resides on a line segment.  ie.
 * return true if the distance from P to the segment is 1 pixel or less.
 *
 * Added by weesan@cs.ucr.edu using algorithm from
 * http://astronomy.swin.edu.au/~pbourke/geometry/pointline/
 */
bool point_on_segment (const Coord &P, const Coord &s, const Coord &e) {
	bool bRet = false;

	// Get all the points
	double x1 = s.x;
	double y1 = s.y;
	double x2 = e.x;
	double y2 = e.y;
	double x3 = P.x;
	double y3 = P.y;

	// Compute denominator of u
	double xDiff = x1 - x2;
	double yDiff = y1 - y2;
	double denom = xDiff * xDiff + yDiff * yDiff;

	do {
		double dDist = 0;

		if (denom == 0.0) {
			/*
			 * Start and end coord are coincident, simply compute the distance
			 * from P to any of the end point
			 */
			dDist = P.distanceTo(s);
			bRet = true;
			break;
		}

		// Compute u with denominator above
		double u = ((x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1)) / denom;
		/*
		 * u has to be [0, 1] (with 10% tolerance) for a point
		 * residing on a segment
		 */
		if (u < 0.0 - INTERSECTION_TOLERANCE_PERCENTAGE ||
			u > 1.0 + INTERSECTION_TOLERANCE_PERCENTAGE) {
			break;
		}

		double x = x1 + u * (x2 - x1);
		double y = y1 + u * (y2 - y1);
		double dist = distance(x, y, x3, y3);

		// If the point is 1 pixel away, we consider it as on the line
		if (dist <= 1.0) {
			bRet = true;
		}
	} while (false);

	return (bRet);
}

//Assumes arc traverses CCW from Arcstart to ArcEnd
bool point_on_arc(Coord P, double r, Coord centr, Coord ArcSp, Coord ArcEp) 
{ 
	if(!equal(centr.distanceTo(P),r)) return false;
	if(!equal(centr.distanceTo(ArcSp),r)) return false;
	if(!equal(centr.distanceTo(ArcEp),r)) return false;

	Coord PA(P-ArcSp);
	Coord BA(ArcEp-ArcSp);

	if(cross(PA.x,PA.y,BA.x,BA.y)>0.0 || equal(cross(PA.x,PA.y,BA.x,BA.y),0.0)) return true;
	return false;
}

double point_to_infiniteline_distance(Coord P, Coord Ls, Coord Le)
{
	static double D,alpha;

	D=P.distanceTo(Le);
	alpha=Angle(P,Le,Ls,Le);

	return fabs(sin(alpha)*D);
}

double point_to_linesegment_distance(Coord P, Coord Ls, Coord Le)
{
	Coord PS(Ls-P);
	Coord PE(Le-P);
	Coord SE(Le-Ls);

	//if perpen. projection of P on the line is between line start and line end...
	if((PS.x*SE.x+PS.y*SE.y)<=0 &&
	   (PE.x*SE.x+PE.y*SE.y)>=0) 
	   return point_to_infiniteline_distance(P, Ls, Le);

	//if perpen. projection of P is not on the line and is closer to line start...
	if((PS.x*SE.x+PS.y*SE.y)>0 &&
	   (PE.x*SE.x+PE.y*SE.y)>0)
	   return P.distanceTo(Ls);

	//if perpen. projection of P is not on the line and is closer to line end...
	   return P.distanceTo(Le);
}

double determinant(Coord sA,Coord eA,Coord sB,Coord eB,double& t,double& u)
{
	static double a,b,c,d,det,fx,fy;

	a=eA.x-sA.x;	b=sB.x-eB.x;     fx=sB.x-sA.x;
	c=eA.y-sA.y;	d=sB.y-eB.y;	 fy=sB.y-sA.y;

	det=a*d-b*c;
	if(fabs(det)<EPSILON) return 0.0;

	t=( d*fx-b*fy)/det;
	u=(-c*fx+a*fy)/det;
	return det;
}

double normalized_gauss_prob(double mean,double stdev,double x)
{
	// Added equal() checking by weesan@cs.ucr.edu
	if (equal(mean, x)) {
		return (1.0);
	} else if (equal(stdev, 0.0)) {
		return (0.0);
	} else {
		//return exp(-1.0*(x-mean)*(x-mean)/(2.0*stdev*stdev)); //normal gaussian distribution
		return exp(-1.0*(x-mean)*(x-mean)*(x-mean)*(x-mean)/(50.0*stdev*stdev*stdev*stdev)); //wider prob distribution
	}
	//see gaussian.mcd to see the differences between these two functions graphically.
	//the wider prob distribution seems to be more suitable
}

// Added by weesan@cs.ucr.edu
// Given 2 points, find out A, B and C in Ax + By + C = 0
void line_equation(Coord &p1, Coord &p2, double *A, double *B, double *C) {
	if (equal(p1.x, p2.x)) {
		// This is the case of x = x1 or x + 0y - x1 = 0
		// Compare Ax + By + C = 0 and x + 0y - x1 = 0
		// => A = 1, B = 0, C = -x1
		*A = 1;
		*B = 0;
		*C = -p1.x;
	} else {
		double m = (p2.y - p1.y) / (p2.x - p1.x);	// The gradient/slope
		double c = p1.y - m * p1.x;					// y = mx + c
	
		// Compare Ax + By + C = 0 and y = mx + c (or mx - y + c = 0)
		// => A = m, B = -1, C = c 
		*A = m;
		*B = -1;
		*C = c;
	}
}