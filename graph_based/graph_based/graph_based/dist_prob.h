// $Id: dist_prob.h,v 1.1 2005/06/22 23:05:41 weesan Exp $

/*
 * dist_prob.h - A distribution-based probability generator
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/7/2005
 */

#ifndef DIST_PROB_H
#define DIST_PROB_H

#include <stdlib.h>
#include <vector>

#define DP_SAMPLE_SIZE  10

using namespace std;

class DistProb : public vector<int> {
private:
    vector<float> cData;

private:
    void parse(vector<float> &data);

public:
    DistProb(vector<float> &data);
    DistProb(int iCount, ...);
    int generate(void) {
		return ((*this)[rand() % DP_SAMPLE_SIZE]);
    }
    float data(void) {
		return (cData[generate()]);
    }
#ifdef DEBUG_DIST_PROB
    void DistProb::dump(void);
#endif
};

#endif // DIST_PROB_H
