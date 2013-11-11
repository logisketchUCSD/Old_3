// $Id: population.cpp,v 1.2 2005/06/24 21:04:32 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#include "stdafx.h"
#include "population.h"

#include <math.h>
#include <assert.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


Population::Population() :
	sumX(0.0),
	sumX2(0.0),
	m(0.0),
	sigma(0.0) {
}

void Population::insert(double x)
{
	push_back(x);
	sumX += x;
	sumX2 += x*x;
	int N = size();
	m=sumX/(double)N;

	if(N>1)
		sigma = sqrt((double(N) * sumX2 - sumX * sumX) /  (double(N) * double(N - 1)));
	else
		sigma = 0.0;
}

double Population::mean() const
{
	return m;
}

double Population::stdev() const
{
	return sigma;
}

double Population::gauss_prob(double x) const
{
	assert(sigma > 0.0);
	return 1.0/sigma/sqrt(2.0*3.141592654)*exp(-1.0*(x-m)*(x-m)/(2.0*sigma*sigma));
}

void Population::clear()
{
	sumX=0.0;
	sumX2=0.0;
	m=0.0;
	sigma=0.0;
	vector<double>::clear();
}

void Population::print()
{
	cout<<"N: "<<size()<<endl;
	for(int i=0;i<size();i++)
		cout<<(*this)[i]<<" ";
	cout<<endl;
	cout<<"Mean: "<<mean()<<endl;
	cout<<"Stdev: "<<stdev()<<endl<<endl;
}