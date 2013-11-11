// $Id: dist_prob.cpp,v 1.2 2005/09/29 00:14:43 weesan Exp $

/*
 * dist_prob.cpp - A distribution-based probability generator
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/7/2005
 */

#include <time.h>
#include <stdarg.h>
#include "dist_prob.h"

DistProb::DistProb(vector<float> &data) {
    parse(data);
}

DistProb::DistProb(int iCount, ...) {
    va_list ap;
    va_start(ap, iCount);

    vector<float> cData;
    for (int i = 0; i < iCount; i++) {
	float fData = va_arg(ap, double);
		cData.push_back(fData);
    }

    va_end(ap);

    parse(cData);
}

void DistProb::parse(vector<float> &data) {
    vector<float> cPercentage(data.size());
    float fTotal = 0;
    
    // Save the raw data and get the total
    int i;
	for (i = 0; i < cPercentage.size(); i++) {
		cData.push_back(data[i]);
	fTotal += data[i];
    }
    
    // Convert it into percentage
    for (i = 0; i < cPercentage.size(); i++) {
	cPercentage[i] = data[i] / fTotal;
    }
    
    for (i = 0; i < cPercentage.size(); i++) {
		for (int j = 0;
			 j < (int)(cPercentage[i] * DP_SAMPLE_SIZE + 0.5);
			 j++) {
			push_back(i);
			if (size() >= DP_SAMPLE_SIZE) {
				break;
			}
		}
    }

    // Take care of the case of precision loss above
    for (i = size(); i < DP_SAMPLE_SIZE; i++) {
		push_back(data.size() - 1);
    }
}

#ifdef DEBUG_DIST_PROB

#define N 100000

void DistProb::dump(void) {
    for (int i = 0; i < size(); i++) {
		printf("%d %d\n", i, (*this)[i]);
    }
    
    vector<int> disp((*this)[size() - 1] + 1);
    for (i = 0; i < N; i++) {
	int gen = generate();
		disp[gen]++;
    }
    
    for (i = 0; i < (*this)[size() - 1] + 1; i++) {
		printf("disp[%d] = %d\n", i, disp[i]);
    }
}
/*
int main(void) {
    //float a[] = { 10, 20, 40, 30, 10 };
    float a[] = { 5, 10, 15, 20, 25, 30, 40, 55, 100 };
    vector<float> data(a, a + sizeof(a) / sizeof(a[0]));

    srandom(time(NULL));
    DistProb cDistProb(data);
    //DistProb cDistProb(3, 5.0, 10.0, 85.0);
    cDistProb.dump();

    return (0);
}
*/
#endif
