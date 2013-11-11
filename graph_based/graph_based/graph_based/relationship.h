// $Id: relationship.h,v 1.6 2006/05/19 00:46:10 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H
//created by Levent Burak Kara

#if _MSC_VER > 1000
#pragma once
#endif 

#include "globals.h"
#include "segment.h"

class Relationship{
public:
	Relationship();
	Relationship(const MySegment* A,const MySegment*B);

	bool  BothAreLines() const;
	Segment_Type AType() const;
	Segment_Type BType() const;
	int NumofIntersections() const;
	double IntAngle() const;
	double IntLocA1() const;
	double IntLocA2() const;
	double IntLocB1() const;
	double IntLocB2() const;

	double IntAngle_stdev() const;
	double IntLocA1_stdev() const;
	double IntLocA2_stdev() const;
	double IntLocB1_stdev() const;
	double IntLocB2_stdev() const;


	void setaType(Segment_Type a);
	void setbType(Segment_Type b);
	void setNumofIntersections(int n);
	void setIntAngle(double a,double stdev);
	void setIntLocA1(double a1,double stdev);
	void setIntLocA2(double a2,double stdev);
	void setIntLocB1(double b1,double stdev);
	void setIntLocB2(double b2,double stdev);

	void print() const;

private:

	Segment_Type aType,bType;
	int numof_intrsctns;
	double intersectionAngle;
	double intlocA1;
	double intlocB1;
	double intlocA2;
	double intlocB2;

	double intersectionAngle_stdev;
	double intlocA1_stdev;
	double intlocB1_stdev;
	double intlocA2_stdev;
	double intlocB2_stdev;
};

class IntrinsicProp{
public:
	// Used by class Definition
	IntrinsicProp(Segment_Type sType,
				  double dFuzzyType,
				  double dFuzzyTypeStdev,
				  double rel_len,
				  double rel_len_stdev,
				  unsigned int uiLocBitmap = 0);
	// Used by class Symbol
	IntrinsicProp(const MySegment* S,double total_len,
				  unsigned int uiLocBitmap = 0);

	Segment_Type SType() const;
	double RelLen() const;
	double RelLen_stdev() const;

	// Added by weesan@cs.ucr.edu
	double fuzzyType(void) const {
		return (dFuzzyType);
	}
	double fuzzyTypeStdev(void) const {
		return (dFuzzyTypeStdev);
	}
	unsigned int locBitmap(void) const {
		return (uiLocBitmap);
	}

	void setsType(Segment_Type a);
	void setRelLen(double rl,double stdev);

	void print() const;

private:
	Segment_Type sType;
	double rel_len;
	double rel_len_stdev;
	// Added by weesan@cs.ucr.edu
	double dFuzzyType;
	double dFuzzyTypeStdev;
	unsigned int uiLocBitmap;
};

class RelationTable{
public:

	RelationTable();
	RelationTable(const RelationTable &r);
	~RelationTable();

	void  set(Relationship& R,int i,int j);
	void  set(const MySegment* A,const MySegment*B,int i,int j);
	void  set(IntrinsicProp& IP,int i);
	void  set(const MySegment* S,double totallen,int i);

	Relationship*  get(int i, int j) const;
	IntrinsicProp* get(int i) const;
	void clear_table();
	void print() const;

private:

	Relationship* relations[MAX_NUMOF_SEGMENTS_INA_SYMBOL][MAX_NUMOF_SEGMENTS_INA_SYMBOL];
	IntrinsicProp* intrscprops[MAX_NUMOF_SEGMENTS_INA_SYMBOL];
};

#endif
