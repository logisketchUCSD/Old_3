// $Id: result.h,v 1.1 2006/05/19 00:46:10 weesan Exp $

/*
 * result.h - A class to keep track of accuracy and elapsed time
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/15/2006
 */

#ifndef RESULT_H
#define RESULT_H

#include "timer.h"

class Result : public Timer {
private:
	int iTotal;
	int iCorrect;

public:
	Result(void) : iTotal(0), iCorrect(0) {
	}
	int total(void) const {
		return (iTotal);
	}
	void incTotal(void) {
		iTotal++;
	}
	int correct(void) const {
		return (iCorrect);
	}
	void incCorrect(void) {
		iCorrect++;
	}
	double accuracy(void) const {
		return ((double)correct() * 100 / total());
	}
	void reset(void) {
		Timer::reset();
		iTotal = iCorrect = 0;
	}
};

#endif // RESULT_H