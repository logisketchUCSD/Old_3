// $Id: algebra.h,v 1.3 2005/09/21 00:46:05 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#ifndef ALGEBRA_H
#define ALGEBRA_H
//created by Levent Burak Kara

#include "Coord.h"
#include "globals.h"
#include <assert.h>


void swap(double& a,double& b);
bool equal(double a,double b);
double sq(double a);
double   dot(double ax,double ay,double bx,double by);
double cross(double ax,double ay,double bx,double by);

double distance(double ax, double ay, double bx, double by);

double Deg2Rad(double d);
double Rad2Deg(double r);

double mean(double sumX,int N);
double std_dev(double sumX,double sumXsqr,int N);


double       Angle(Coord sA,Coord eA,Coord sB,Coord eB);
double Acute_Angle(Coord sA,Coord eA,Coord sB,Coord eB);
bool      Parallel(Coord sA,Coord eA,Coord sB,Coord eB);
bool Perpendicular(Coord sA,Coord eA,Coord sB,Coord eB);

int  LineSegLineSeg_intersect(Coord sA,Coord eA,Coord sB,Coord eB, Coord& intP);
int   LineSegCircle_intersect(Coord sL,Coord eL,Coord cenB,double r, Coord& intP1, Coord& intP2);
int    CircleCircle_intersect(Coord cenA,double rA,Coord cenB,double rB, Coord& intP1, Coord& intP2);

bool   point_on_segment(const Coord &P, const Coord &s, const Coord &e);
bool   point_on_arc(Coord P, double r, Coord centr, Coord ArcSp, Coord ArcEp);
double point_to_infiniteline_distance(Coord P, Coord Ls, Coord Le);
double point_to_linesegment_distance(Coord P, Coord Ls, Coord Le);
double determinant(Coord sA,Coord eA,Coord sB,Coord eB,double& t,double& u);

double normalized_gauss_prob(double mean,double stdev,double x);
void line_equation(Coord &p1, Coord &p2, double *A, double *B, double *C);

#endif