// $Id: stats.h,v 1.1 2006/05/19 00:46:11 weesan Exp $

/*
 * stats.h - Doing statistics.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/16/2006
 */

#ifndef STATS_H
#define STATS_H

#include <math.h>
#include <vector>

using namespace std;

class Stats : vector<double> {
private:
    double dSum;
    double dX2Sum;
    
public:
    Stats(void) : dSum(0), dX2Sum(0) {
    }
    double sum(void) const {
		return (dSum);
    }
    // Return the mu
    double mean(void) const {
		return (sum() / size());
    }
    // Return the sigma
    double stdDev(void) const {
		if (size() == 1) {
		    return (0);
		} else {
			return (sqrt((size() * dX2Sum - sum() * sum()) /
					(size() * (size() - 1))));
		}
    }
    void add(float x) {
		push_back(x);
		dSum += x;
		dX2Sum += (x * x);
    }
};

#endif // STATS_H
