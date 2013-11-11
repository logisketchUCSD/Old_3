// $Id: relationship.cpp,v 1.6 2006/05/19 00:46:10 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#include "stdafx.h"
#include "relationship.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

Relationship::Relationship() :
	aType(UNKNOWN_TYPE),
	bType(UNKNOWN_TYPE),
	intersectionAngle(0),
	intlocA1(0),
	intlocB1(0),
	intlocA2(0),
	intlocB2(0),
	numof_intrsctns(0),
	intersectionAngle_stdev(0),
	intlocA1_stdev(0),
	intlocB1_stdev(0),
	intlocA2_stdev(0),
	intlocB2_stdev(0) {
}

Relationship::Relationship(const MySegment* A,const MySegment*B) :
	aType(UNKNOWN_TYPE),
	bType(UNKNOWN_TYPE),
	intersectionAngle(0),
	intlocA1(0),
	intlocB1(0),
	intlocA2(0),
	intlocB2(0),
	numof_intrsctns(0),
	intersectionAngle_stdev(0),
	intlocA1_stdev(0),
	intlocB1_stdev(0),
	intlocA2_stdev(0),
	intlocB2_stdev(0) {

	double a1,b1,a2,b2;
	aType=A->Type();
	bType=B->Type();

	if(BothAreLines())	
	{
		setIntAngle(A->Line_Line_Acute_Angle(*B),EPSILON);
	}
	if(A->intersectsOnce(*B,a1,b1))
	{
		setIntLocA1(a1,EPSILON);
		setIntLocB1(b1,EPSILON);
		setIntLocA2(0,EPSILON); //dummy 0
		setIntLocB2(0,EPSILON); //dummy 0
		setNumofIntersections(1);
	}
	if(A->intersectsTwice(*B,a1,b1,a2,b2))
	{
		setIntLocA1(a1,EPSILON);
		setIntLocB1(b1,EPSILON);
		setIntLocA2(a2,EPSILON);
		setIntLocB2(b2,EPSILON);
		setNumofIntersections(2);
	}
}

bool Relationship::BothAreLines() const
{
	return (aType==LINE && bType==LINE);
}
Segment_Type Relationship::AType() const
{
	return aType;
}
Segment_Type Relationship::BType() const
{
	return bType;
}
int   Relationship::NumofIntersections() const
{
	return numof_intrsctns;
}
double Relationship::IntAngle() const
{
	return intersectionAngle;
}
double Relationship::IntLocA1() const
{
	return intlocA1;
}
double Relationship::IntLocA2() const
{
	return intlocA2;
}
double Relationship::IntLocB1() const
{
	return intlocB1;
}
double Relationship::IntLocB2() const
{
	return intlocB2;
}
double Relationship::IntAngle_stdev() const
{
	return intersectionAngle_stdev;
}
double Relationship::IntLocA1_stdev() const
{
	return intlocA1_stdev;
}
double Relationship::IntLocA2_stdev() const
{
	return intlocA2_stdev;
}
double Relationship::IntLocB1_stdev() const
{
	return intlocB1_stdev;
}
double Relationship::IntLocB2_stdev() const
{
	return intlocB2_stdev;
}
void Relationship::setaType(Segment_Type a)
{
	aType=a;
}
void Relationship::setbType(Segment_Type b)
{
	bType=b;
}
void Relationship::setNumofIntersections(int n)
{
	assert(n>=0 && n<=2);
	numof_intrsctns=n;
}
void Relationship::setIntAngle(double a,double stdev)
{
	intersectionAngle=a;
	intersectionAngle_stdev=stdev;
}
void Relationship::setIntLocA1(double a1,double stdev)
{
	intlocA1=a1;
	intlocA1_stdev=stdev;
}
void Relationship::setIntLocA2(double a2,double stdev)
{
	intlocA2=a2;
	intlocA2_stdev=stdev;
}
void Relationship::setIntLocB1(double b1,double stdev)
{
	intlocB1=b1;
	intlocB1_stdev=stdev;
}
void Relationship::setIntLocB2(double b2,double stdev)
{
	intlocB2=b2;
	intlocB2_stdev=stdev;
}


void Relationship::print() const
{
	cout<<"--------------\n";
	/*if(AType()==LINE) cout<<"Line - ";
	else              cout<<"Arc  - ";
	if(BType()==LINE) cout<<"Line\n";
	else              cout<<"Arc\n";*/
	if(BothAreLines()) 
	{
		cout<<"Intersection Angle and Stddev of infinite lines: "<<Rad2Deg(intersectionAngle)<<" , "<<Rad2Deg(intersectionAngle_stdev)<<endl;
	}
	/*cout<<"Intersect at "<<numof_intrsctns<<" locations:\n";
	if(numof_intrsctns==1)	
	{
		cout<<"A1: "<<intlocA1<<"  stdev:"<<intlocA1_stdev<<endl;
		cout<<"B1: "<<intlocB1<<"  stdev:"<<intlocB1_stdev<<endl;
	}
	if(numof_intrsctns==2)
	{
		cout<<"A1: "<<intlocA1<<"  stdev:"<<intlocA1_stdev<<endl;
		cout<<"A2: "<<intlocA2<<"  stdev:"<<intlocA2_stdev<<endl;
		cout<<"B1: "<<intlocB1<<"  stdev:"<<intlocB1_stdev<<endl;
		cout<<"B2: "<<intlocB2<<"  stdev:"<<intlocB2_stdev<<endl;
	}
	cout<<endl;*/
}

//////////////////////////////////

IntrinsicProp::IntrinsicProp(Segment_Type sType,
							 double dFuzzyType,
							 double dFuzzyTypeStdev,
							 double rel_len,
							 double rel_len_stdev,
							 unsigned int uiLocBitmap) :
	sType(sType),
	dFuzzyType(dFuzzyType),
	dFuzzyTypeStdev(dFuzzyTypeStdev),
	rel_len(rel_len),
	rel_len_stdev(rel_len_stdev),
	uiLocBitmap(uiLocBitmap) {
}

IntrinsicProp::IntrinsicProp(const MySegment* S,double total_len,
							 unsigned int uiLocBitmap) :
	sType(S->Type()),
	dFuzzyType(S->fuzzyType()),
	dFuzzyTypeStdev(0),
	rel_len(S->RelLength(total_len)),
	rel_len_stdev(EPSILON),
	uiLocBitmap(uiLocBitmap) {
}

Segment_Type IntrinsicProp::SType() const
{
	return sType;
}

double IntrinsicProp::RelLen() const
{
	return rel_len;
}

double IntrinsicProp::RelLen_stdev() const
{
	return rel_len_stdev;
}

void IntrinsicProp::setsType(Segment_Type a)
{
	sType=a;
}

void IntrinsicProp::setRelLen(double rl,double stdev)
{
	assert(rl>=0.0 && rl<=1.0);
	rel_len=rl;
	rel_len_stdev=stdev;
}

void IntrinsicProp::print() const
{
	cout<<"Int Prop: "<<endl;
	cout<<"Rel len= "<<rel_len<<endl;
	cout<<"Rel len stdev= "<<rel_len_stdev<<endl;
}

//////////////////////////////////

RelationTable::RelationTable()
{
	for(int i=0;i<MAX_NUMOF_SEGMENTS_INA_SYMBOL;i++)
	{
		intrscprops[i]=NULL;
		for(int j=0;j<MAX_NUMOF_SEGMENTS_INA_SYMBOL;j++)
		{
			relations[i][j]=NULL;
		}
	}
}

RelationTable::~RelationTable()
{
	clear_table();
}

RelationTable::RelationTable(const RelationTable &r) {
	printf("here\n");
	for (int i = 0; i < MAX_NUMOF_SEGMENTS_INA_SYMBOL; i++) {
		if (r.intrscprops[i]) {
			intrscprops[i] = new IntrinsicProp(*r.intrscprops[i]);
		} else {
			intrscprops[i] = NULL;
		}

		for (int j = 0; j < MAX_NUMOF_SEGMENTS_INA_SYMBOL; j++) {
			if (r.relations[i][j]) {
				relations[i][j] = new Relationship(*r.relations[i][j]);
			} else {
				relations[i][j] = NULL;
			}
		}
	}
}

void RelationTable::set(Relationship& R,int i,int j)
{
	assert(i>=0 && j>=0 && i<MAX_NUMOF_SEGMENTS_INA_SYMBOL && j<MAX_NUMOF_SEGMENTS_INA_SYMBOL);
	assert(i<j);
	assert(relations[i][j] == NULL);

	relations[i][j]=new Relationship(R);
}

void RelationTable::set(const MySegment* A,const MySegment*B,int i,int j)
{
	assert(i>=0 && j>=0 && i<MAX_NUMOF_SEGMENTS_INA_SYMBOL && j<MAX_NUMOF_SEGMENTS_INA_SYMBOL);
	assert(i<j);
	assert(relations[i][j] == NULL);

	relations[i][j]=new Relationship(A,B);
}

// Used by class Definition to assign relative length of each collective
// segment from the training samples
void  RelationTable::set(IntrinsicProp& IP,int i)
{
	assert(i>=0 && i<MAX_NUMOF_SEGMENTS_INA_SYMBOL);
	assert(intrscprops[i] == NULL);

	intrscprops[i]=new IntrinsicProp(IP);
}

// Used by class Symbol to assign relative length of each segment of the symbol
void  RelationTable::set(const MySegment* S,double totallen,int i)
{
	assert(i>=0 && i<MAX_NUMOF_SEGMENTS_INA_SYMBOL);
	assert(intrscprops[i] == NULL);

	intrscprops[i]=new IntrinsicProp(S,totallen);
}

Relationship* RelationTable::get(int i, int j) const
{
	if (i >= 0 && j >= 0 &&
		i < MAX_NUMOF_SEGMENTS_INA_SYMBOL &&
		j < MAX_NUMOF_SEGMENTS_INA_SYMBOL &&
		i < j) {
		return (relations[i][j]);
	} else {
		return (NULL);
	}
}

IntrinsicProp* RelationTable::get(int i) const {
	if (i >= 0  && i < MAX_NUMOF_SEGMENTS_INA_SYMBOL) {
		return (intrscprops[i]);
	} else {
		return (NULL);
	}
}

void RelationTable::clear_table()
{
	for(int i=0;i<MAX_NUMOF_SEGMENTS_INA_SYMBOL;i++)
	{
		if(intrscprops[i]!=NULL)
		{
			delete intrscprops[i];
			intrscprops[i]=NULL;
		}

		for(int j=0;j<MAX_NUMOF_SEGMENTS_INA_SYMBOL;j++)
		{
			if(relations[i][j]!=NULL)
			{
				delete relations[i][j];
				relations[i][j]=NULL;
			}
		}
	}
}

void RelationTable::print() const
{
	for(int i=0;i<MAX_NUMOF_SEGMENTS_INA_SYMBOL;i++)
	{
		if(intrscprops[i]!=NULL)
		{
			intrscprops[i]->print();
		}
		for(int j=i+1;j<MAX_NUMOF_SEGMENTS_INA_SYMBOL;j++)
		{
			if(relations[i][j]!=NULL)
			{
				cout<<"\nSegment "<<i<<"-"<<j<<endl;
				relations[i][j]->print();
			}
		}
	}

}