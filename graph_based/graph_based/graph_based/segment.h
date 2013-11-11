// $Id: segment.h,v 1.12 2006/12/11 06:34:17 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#ifndef SEGMENT_H
#define SEGMENT_H
//created by Levent Burak Kara

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "object.h"
#include "Coord.h"
#include "globals.h"
#include "algebra.h"

#define SEGMENT_BETWEEN(x, a, b)	((x) >= (a) && (x) <= b)
#define SEGMENT_X_BETWEEN(a, b)		(SEGMENT_BETWEEN((a)->StartPoint().x,	\
													 (b)->StartPoint().x,	\
													 (b)->EndPoint().x) ||	\
									 SEGMENT_BETWEEN((a)->EndPoint().x,		\
													 (b)->StartPoint().x,	\
													 (b)->EndPoint().x))
#define SEGMENT_X_OVERLAPPED(a, b)	(SEGMENT_X_BETWEEN(a, b) || \
									 SEGMENT_X_BETWEEN(b, a))

//Constructor:
//startx,starty,endx,endy,centerx,centery,radius,startangle,endangle,circle_direction,original_segment_type
//startangle has to be [0,2pi)
//  endangle has to be [0,2pi)
//Circle_Dir: CW or CCW

class MySegment : public Object {
public:

	// Added by weesan@cs.ucr.edu
	MySegment(void);
	MySegment(double sx,double sy,double ex,double ey,double cx,double cy,double r,double sa,double ea,
																		Circle_Dir cd,Segment_Type t);
	~MySegment();

	//const MySegment& operator= (const MySegment&);

	Segment_Type Type() const;
	double fuzzyType(void) const;

	double Length() const;
	double RelLength(double totallength) const;
	double Line_Line_Acute_Angle(const MySegment& S) const;

	bool intersectsOnce(const MySegment& S) const;
	bool intersectsTwice(const MySegment& S) const;
	bool intersectsOnce(const MySegment& S, double& percentLocA, double& percentLocB) const;
	bool intersectsTwice(const MySegment& S,double& percentLocA1,double& percentLocB1,double& percentLocA2,double& percentLocB2) const;

	Coord StartPoint() const;
	Coord   EndPoint() const;
	Coord     Center() const;
	double    Radius() const;
	double SmallAngle() const;
	double   BigAngle() const;
/*
	// Not being used, commented out by weesan@cs.ucr.edu
	void setError(double e);
	double getError() const;
*/
	// Moved from private and changed to be const ref. by weesan@cs.ucr.edu
	bool PointOnLine(const Coord &P) const;
	bool PointOnArc(const Coord &P) const;

	// Added by weesan@cs.ucr.edu
	int segNo(void) const {
		return (iSegNo);
	}
	MySegment *segNo(int _iSegNo) {
		iSegNo = _iSegNo;
		return (this);
	}
	unsigned int score(void) const {
		return (uiScore);
	}
	void score(unsigned int _uiScore) {
		uiScore = _uiScore;
	}
	unsigned int locBitmap(void) const {
		return (uiLocBitmap);
	}
	void locBitmap(unsigned int _uiLocBitmap) {
		uiLocBitmap = _uiLocBitmap;
	}
	// Flip the drawing order of the segment
	void flip(void);

protected:
	void write(std::ostream &os) const;
	void read(std::istream &is);

private:
	// Will be updated by Symbol::Add_Segment() - added by weesan@cs.ucr.edu
	int iSegNo;
	// Will be used by Symbol::sort() to assign score for sorting segments
	unsigned int uiScore;
	// Used to keep track of the approximation segment location of a symbol
	unsigned int uiLocBitmap;

	Coord startp;
	Coord endp;
	Coord center;
	double radius;
	double smlang,bigang;//make sure smallang is between [0,2PI) and bigang [0,4Pi) and bigangle >= smallangle
	Circle_Dir arc_dir;
	double error;

	Segment_Type    orig_type;
	Segment_Type current_type;

	Coord ArcSmallP() const;
	Coord ArcBigP() const;
	double Percent_Loc(const Coord &P) const;
};

#endif

