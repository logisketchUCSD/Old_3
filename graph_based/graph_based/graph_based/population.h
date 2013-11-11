// $Id: population.h,v 1.2 2005/06/24 21:04:32 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#ifndef POPULATION_H
#define POPULATION_H
//created by Levent Burak Kara

#if _MSC_VER > 1000
#pragma once
#endif 

#include <iostream>
#include <vector>
using namespace std;

class Population : public vector<double> {
public:
	Population();
	void insert(double x);
	double  mean() const;
	double stdev() const;
	double gauss_prob(double x) const;
	void clear();
	void print();

private:
	double sumX;  //sum X
	double sumX2; //sum Xsquare
	double m;     //mean
	double sigma; //standard deviation
};


#endif
