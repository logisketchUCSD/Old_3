// $Id: segment.cpp,v 1.12 2006/12/11 06:34:17 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#include "stdafx.h"
#include "segment.h"
#include "debug.h"

#include <math.h>
#include <assert.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Added by weesan@cs.ucr.edu
MySegment::MySegment(void) :
	Object("Segment"),
	iSegNo(0),
	uiScore(0),
	uiLocBitmap(0),
	radius(0.0),
	smlang(0.0),
	bigang(0.0),
	arc_dir(UNKNOWN_DIR),
	current_type(UNKNOWN_TYPE),
	orig_type(UNKNOWN_TYPE),
	error(0.0) {
}

MySegment::MySegment(double sx,double sy,double ex,double ey,double cx,double cy,
					 double r,double sa,double ea, Circle_Dir cd,Segment_Type t) :
	Object("Segment"),
	iSegNo(0),
	uiScore(0),
	uiLocBitmap(0) {
	startp.set(sx,sy);
	endp.set(ex,ey);
	center.set(cx,cy);
	radius=r;
	arc_dir=cd;
	smlang = bigang = 0;	// Added by weesan@cs.ucr.edu
	current_type=orig_type=t;
	error = 0.0;
/*
	assert(!equal(radius,0.0));
	assert(radius>0.0);
	assert(!equal(sa,ea));
	assert(sa>=0.0 && sa<=2.0*PI && ea>=0.0 && ea<=2.0*PI);
*/
	if(sa < ea && arc_dir==CCW)
	{
		smlang=sa;
		bigang=ea;
	}

	if(sa > ea && arc_dir==CCW)
	{
		smlang=sa;
		bigang=ea+2.0*PI;
	}

	if(sa < ea && arc_dir==CW)
	{
		smlang=ea;
		bigang=sa+2.0*PI;
	}

	if(sa > ea && arc_dir==CW)
	{
		smlang=ea;
		bigang=sa;
	}
	//MAKE SURE smallang is between [0,2Pi) and bigang [0,4Pi) and bigangle >= smallangle
	//for documentation, go to http://www.magic-software.com/Intersection2D.html
	//                         http://www.magic-software.com/Documentation/IntersectionLin2Cir2.pdf
	assert(smlang>=0.0 && smlang<=2.0*PI);
	assert(bigang>=0.0 && bigang<=4.0*PI);
	assert(smlang <= bigang);
}

MySegment::~MySegment()
{
}
/*
const MySegment& MySegment::operator= (const MySegment& S)
{
	if(this==&S) return *this;
	startp=S.startp;
	endp=S.endp;
	center=S.center;
	radius=S.radius;
	smlang=S.smlang;
	bigang=S.bigang;
	arc_dir=S.arc_dir;
	current_type=S.current_type;
	orig_type=S.orig_type;
	error=S.error;
	return *this;
}
*/
double MySegment::Length() const
{
	if(current_type==LINE)
	{
		return startp.distanceTo(endp);
	}
	else //i.e., if it is an ARC
	{
		return fabs(bigang-smlang)*radius;
	}
}

double MySegment::RelLength(double totallength) const
{
	if(equal(totallength,0.0)) return 0.0;
	return Length()/totallength;
}

Segment_Type MySegment::Type() const
{
	return current_type;
}

// Added by weesan@cs.ucr.edu
double MySegment::fuzzyType(void) const {
	double dType = 0;

	switch (orig_type) {
	case LINE:
		dType = 0;
		break;
	case ARC: {
		double dDiff = Rad2Deg(bigang - smlang);
		if (dDiff > 72) {
			dType = 1;
		} else {
			dType = (dDiff - 36) / 36;
		}
		break;
	}
	default:
		break;
	}

	return (dType);
}

bool MySegment::intersectsOnce(const MySegment& S) const
{
	double pa = 0, pb = 0;
	return intersectsOnce(S, pa,pb);
}
bool MySegment::intersectsTwice(const MySegment& S) const
{
	double pa1 = 0, pb1 = 0, pa2 = 0, pb2 = 0;
	return intersectsTwice(S,pa1,pb1,pa2,pb2);
}


bool MySegment::intersectsOnce(const MySegment& S, double& relposA, double& relposB) const
{
	Coord P1,P2;

	//LINE-LINE
	if(current_type==LINE && S.current_type==LINE)
	{
		if(LineSegLineSeg_intersect(startp,endp,S.startp,S.endp,P1)==1)
		{
			relposA=  Percent_Loc(P1);	
			relposB=S.Percent_Loc(P1);
			return true;
		}
		return false;
	}

	//ARC-ARC
	if(current_type==ARC && S.current_type==ARC)
	{
		if(CircleCircle_intersect(center,radius,S.center,S.radius, P1, P2)==2)
		{
			if(S.PointOnArc(P1) && PointOnArc(P1) && (!S.PointOnArc(P2) || !PointOnArc(P2)) )
			{
				relposA=  Percent_Loc(P1);
				relposB=S.Percent_Loc(P1);
				return true;
			}
			if((!S.PointOnArc(P1) || !PointOnArc(P1)) && S.PointOnArc(P2) && PointOnArc(P2) )
			{
				relposA=  Percent_Loc(P2);
				relposB=S.Percent_Loc(P2);
				return true;
			}
			return false;
		}

		if(CircleCircle_intersect(center,radius,S.center,S.radius, P1, P2)==1)
		{	
			if(PointOnArc(P1) && S.PointOnArc(P1))
			{
				relposA=  Percent_Loc(P1);
				relposB=S.Percent_Loc(P1);
				return true;
			}
			return false;
		}
		return false;
	}

	//LINE-ARC
	if(current_type==LINE && S.current_type==ARC)
	{	
		int in = LineSegCircle_intersect(startp,endp,S.center,S.radius, P1, P2);
		if (in == 2) {
			if(S.PointOnArc(P1) && !S.PointOnArc(P2))
			{
				relposA=  Percent_Loc(P1); 
				relposB=S.Percent_Loc(P1);
				return true;
			}
			if(!S.PointOnArc(P1) && S.PointOnArc(P2))
			{
				relposA=  Percent_Loc(P2); 
				relposB=S.Percent_Loc(P2);
				return true;
			}
			return false;
		}


		if(in == 1) {
			if(S.PointOnArc(P1))
			{
				relposA=  Percent_Loc(P1); 
				relposB=S.Percent_Loc(P1);
				return true;
			}
			return false;
		}
		return false;
	}

	//ARC-LINE
	if(current_type==ARC && S.current_type==LINE)
	{		
		int in = LineSegCircle_intersect(S.startp,S.endp,center,radius, P1, P2);
		if (in == 2) {
			if( PointOnArc(P1) && !PointOnArc(P2))
			{
				relposA=  Percent_Loc(P1); 
				relposB=S.Percent_Loc(P1);
				return true;
			}
			if(!PointOnArc(P1) &&  PointOnArc(P2))
			{
				relposA=  Percent_Loc(P2); 
				relposB=S.Percent_Loc(P2);
				return true;
			}
			return false;
		}


		if(in == 1)
		{
			if(PointOnArc(P1))
			{
				relposA=  Percent_Loc(P1); 
				relposB=S.Percent_Loc(P1);
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

bool MySegment::intersectsTwice(const MySegment& S,double& relposA1,double& relposB1,double& relposA2,double& relposB2) const
{
	static Coord P1,P2;

	if(current_type==LINE && S.current_type==LINE)
	{
		return false;
	}
	if(current_type==ARC && S.current_type==ARC)
	{
		if(CircleCircle_intersect(center,radius,S.center,S.radius, P1, P2)==2)
		{
			if(PointOnArc(P1) && S.PointOnArc(P1) && PointOnArc(P2) && S.PointOnArc(P2))
			{
				relposA1=  Percent_Loc(P1);
				relposB1=S.Percent_Loc(P1);
				relposA2=  Percent_Loc(P2);
				relposB2=S.Percent_Loc(P2);
				return true;
			}
			return false;
		}
		return false;
	}
	
	if(current_type==LINE && S.current_type==ARC)
	{
		
		if(LineSegCircle_intersect(startp,endp,S.center,S.radius, P1, P2)==2)
		{
			if(S.PointOnArc(P1) && S.PointOnArc(P2))
			{
				relposA1=  Percent_Loc(P1); 
				relposB1=S.Percent_Loc(P1);
				relposA2=  Percent_Loc(P2); 
				relposB2=S.Percent_Loc(P2);
				return true;
			}
			return false;
		}
		return false;
	}
	if(current_type==ARC && S.current_type==LINE)
	{
		if(LineSegCircle_intersect(S.startp,S.endp,center,radius,P1,P2)==2)
		{
			if(PointOnArc(P1) && PointOnArc(P2))
			{
				relposA1=  Percent_Loc(P1);
				relposB1=S.Percent_Loc(P1); 
				relposA2=  Percent_Loc(P2);
				relposB2=S.Percent_Loc(P2);
				return true;
			}
			return false;
		}
		return false;
	}

	return false;
}

double MySegment::Line_Line_Acute_Angle(const MySegment& S) const
{
	return Acute_Angle(startp,endp,S.startp,S.endp);
}


Coord MySegment::ArcSmallP() const
{	static Coord P;
	P.x=center.x+radius*cos(smlang);
	P.y=center.y+radius*sin(smlang);
	if(equal(P.x,0.0)) P.x=0.0;
	if(equal(P.y,0.0)) P.y=0.0;
	return P;
}
Coord MySegment::ArcBigP() const
{
	static Coord P;
	P.x=center.x+radius*cos(bigang);
	P.y=center.y+radius*sin(bigang);
	if(equal(P.x,0.0)) P.x=0.0;
	if(equal(P.y,0.0)) P.y=0.0;
	return P;
}

double MySegment::Percent_Loc(const Coord &P) const
{
	double Pangle,PS,PE,L,perloc;

	L=Length();

	if(equal(L,0.0)) return 0;

	if(current_type==LINE)
	{
		assert(PointOnLine(P));
		PS=P.distanceTo(startp);	
		PE=P.distanceTo(endp);
		if(equal(PE-PS,L)) 
			return -1.0*PS/L; 
		else
			return PS/L;
	}
	else
	{   
		assert(PointOnArc(P));
		
		Pangle=atan2(P.y-center.y,P.x-center.x);
		if(Pangle<smlang && !equal(Pangle,smlang)) Pangle+=2*PI; //so that
		
		perloc=fabs((Pangle-smlang)/(bigang-smlang));

		if(arc_dir==CCW) return perloc;
		else             return fabs(1.0-perloc);
	}
}

// Fixed by weesan@cs.ucr.edu
bool MySegment::PointOnLine(const Coord &P) const {
	static double PS,PE,L;
	L=Length();
	PS=P.distanceTo(startp);
	PE=P.distanceTo(endp);

	if(equal(L,PS+PE) || equal(L,fabs(PS-PE))) 
		return true; 
	
	return false;
}

bool MySegment::PointOnArc(const Coord &P) const
{
	return point_on_arc(P, radius, center, ArcSmallP(), ArcBigP());
}

void MySegment::flip(void) {
	switch (Type()) {
	case LINE: {
		Coord tmp = startp;
		startp = endp;
		endp = tmp;
		break;
	}
	case ARC:
		if (arc_dir == CW) {
			arc_dir = CCW;
		} else {
			arc_dir = CW;
		}
		break;
	default:
		break;
	}
}

// Modified by weesan@cs.ucr.edu to return start point of arc as well
Coord MySegment::StartPoint() const
{
	if (Type() == ARC) {
		// Added by weesan@cs.ucr.edu
		if (arc_dir == CW) {
			double x = Center().x + Radius() * cos(BigAngle());
			double y = Center().y + Radius() * sin(BigAngle());
			return (Coord(x, y));
		} else {
			double x = Center().x + Radius() * cos(SmallAngle());
			double y = Center().y + Radius() * sin(SmallAngle());
			return (Coord(x, y));
		}
	} else {
		return startp;
	}
}

// Modified by weesan@cs.ucr.edu to return end point of arc as well
Coord MySegment::EndPoint() const
{
	if (Type() == ARC) {
		// Added by weesan@cs.ucr.edu
		if (arc_dir == CW) {
			double x = Center().x + Radius() * cos(SmallAngle());
			double y = Center().y + Radius() * sin(SmallAngle());
			return (Coord(x, y));
		} else {
			double x = Center().x + Radius() * cos(BigAngle());
			double y = Center().y + Radius() * sin(BigAngle());
			return (Coord(x, y));
		}
	} else {
		return endp;
	}
}
Coord MySegment::Center() const
{
	return center;
}
double MySegment::Radius() const
{
	return radius;
}
double MySegment::SmallAngle() const
{
	return smlang;
}
double MySegment::BigAngle() const
{
	return bigang;
}

/*
// Not being used, commented out by weesan@cs.ucr.edu
void MySegment::setError(double e)
{
	error = e;
}
double MySegment::getError() const
{
	return error;
}
*/

void MySegment::write(std::ostream &os) const {
/*
	os << '\t' << name() << " = {" << endl;
	os << "\t\tstart = " << startp << endl
	   << "\t\tend = " << endp << endl
	   << "\t\tcenter = " << center << endl
	   << "\t\tradius = " << radius << endl
	   << "\t\tsmall_angle = " << smlang << endl
	   << "\t\tbig_angle = " << bigang << endl
	   << "\t\tarc_dir = "<< arc_dir << endl
	   << "\t\ttype = " << orig_type << endl;
	os << "\t}";
*/
	os << '\t' << name() << " = {" << endl;
	os << "\t\tstart = " << startp << endl
	   << "\t\tend = " << endp << endl
	   << "\t\tcenter = " << center << endl
	   << "\t\tradius = " << radius << endl
	   << "\t\tsmall_angle = " << Rad2Deg(smlang) << endl
	   << "\t\tbig_angle = " << Rad2Deg(bigang) << endl
	   << "\t\tdiff_angle = " << Rad2Deg(bigang - smlang) << endl
	   << "\t\tarc_dir = "<< arc_dir << endl
	   << "\t\ttype = " << orig_type << endl
	   << "\t\tfuzzy_type = " << fuzzyType() << endl;
	os << "\t}";
}

void MySegment::read(std::istream &is) {
	if (!is) {
		return;
	}

	char buf[256];

	// segment = {
	is.getline(buf, sizeof(buf));
	while (is) {
		char attr[256], eq;
		int i = 0;

		// Get the attr
		is >> attr;

		// If end of segment, break out of loop
		if (*attr == '}') {
			// Ignore the rest of the line including \r\n
			is.ignore();
			break;
		}

		// Get the '='
		is >> eq;

		// Get the value
		switch (*attr) {
		case 's':
			if (strcmp(attr, "start") == 0) {
				is >> startp;
			} else if (strcmp(attr, "small_angle") == 0) {
				is >> smlang;
			} else {
				fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			}
			break;
		case 'e':
			if (strcmp(attr, "end") == 0) {
				is >> endp;
			} else {
				fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			}
			break;
		case 'c':
			if (strcmp(attr, "center") == 0) {
				is >> center;
			} else {
				fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			}
			break;
		case 'r':
			if (strcmp(attr, "radius") == 0) {
				is >> radius;
			} else {
				fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			}
			break;
		case 'b':
			if (strcmp(attr, "big_angle") == 0) {
				is >> bigang;
			} else {
				fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			}
			break;
		case 'a':
			if (strcmp(attr, "arc_dir") == 0) {
				is >> i;
				arc_dir = (Circle_Dir)i;
			} else {
				fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			}
			break;
		case 't':
			if (strcmp(attr, "type") == 0) {
				is >> i;
				orig_type = current_type = (Segment_Type)i;
			} else {
				fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			}
			break;
		default:
			fprintf(stderr, "Unknown attribute: '%s'\n", attr);
			break;
		}
	}
}
