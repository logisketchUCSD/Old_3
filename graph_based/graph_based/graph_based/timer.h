// $Id: timer.h,v 1.3 2006/12/23 08:37:40 weesan Exp $

/*
 * timer.h - A timer class to compute the elapse time
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/21/2005
 */

#ifndef TIMER_H
#define TIMER_H

#include "stdafx.h"
#include "memory.h"

class Timer {
private:
	LARGE_INTEGER tFreq;
	LARGE_INTEGER tStart, tFinish;
	double dTotal;
	int iCount;

private:
	// Sum up the last elapsed time
	void sum(void) {
		dTotal += elapsedTime();
		iCount++;
	}

public:
	Timer(void) : dTotal(0), iCount(0) {
		QueryPerformanceFrequency(&tFreq);
		reset();
	}
	double totalTime(void) const {
		return (dTotal);
	}
	int count(void) const {
		return (iCount);
	}
	void start(void) {
		QueryPerformanceCounter(&tStart);
	}
	void finish(void) {
		QueryPerformanceCounter(&tFinish);
		sum();
	}
	// Return elapsed time in ms
	double elapsedTime(void) {
		return ((tFinish.QuadPart - tStart.QuadPart) * 1000.0 / tFreq.QuadPart);
	}
	double average(void) {
		return (dTotal / iCount);
	}
	void reset(void) {
		memset(&tStart, 0, sizeof(tStart));
		memset(&tFinish, 0, sizeof(tFinish));
		dTotal = 0;
		iCount = 0;
	}
};

#endif // TIMER_H