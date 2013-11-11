// $Id: symbol.h,v 1.33 2006/12/27 22:25:26 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#ifndef SYMBOL_H
#define SYMBOL_H
//created by Levent Burak Kara

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "segment.h"
#include "globals.h"
#include "relationship.h"
#include "object.h"
#include "scandir.h"
#include <vector>
#include <fstream>
#include <queue>
#include <map>
#include <string>

#define SYMBOL_RANDOMIZE_MAX    10
#define SYMBOL_CHUCK_SIZE	    4

using namespace std;


// Added by weesan@cs.ucr.edu
// Forward declaration
class Definition;

class BestDrawOrder {
private:
	double dWeight;
	vector<int> cOrder;

public:
	BestDrawOrder(double dWeight, vector<int>& cOrder) :
		dWeight(dWeight),
		cOrder(cOrder) {
	}
	double weight(void) const {
		return (dWeight);
	}
	vector<int> &order(void) {
		return (cOrder);
	}
	bool operator<(const BestDrawOrder &b) const {
		return (weight() < b.weight());
	}
};

class Symbol : public Object, public vector<MySegment *> {
public:
	Symbol();
	Symbol(const Symbol &s);
	// Added by weesan@cs.ucr.edu
	~Symbol(void) {
		for (int i = 0; i < size(); i++) {
			delete (*this)[i];
		}
	}
	void Add_Segment(MySegment* S);
	void Process();
	void Shuffle_Segments(int iDefSize = 0);
	void randomize(int iCount = SYMBOL_RANDOMIZE_MAX) {
		// Randomize it by swaping 2 segments each time
		for (int i = 0; i < iCount; i++) {
			int a = rand() % size();
			int b = rand() % size();
			if (a != b) {
				MySegment *temp = (*this)[a];
				(*this)[a] = (*this)[b];
				(*this)[b] = temp;
			}
		}
		// After that, need to rebuilt relationship table
		Process();
		//restoreOrigOrder();
		segindexmap.clear();
		for (int i = 0; i < size(); i++) {
			segindexmap.push_back(i);
		}
	}
	// Added by weesan@cs.ucr.edu
	const char *drawOrder(void) const;
	void saveBestDrawOrder(double dWeight) {
		cBestDrawOrder.push(BestDrawOrder(dWeight, segindexmap));
		// Also save best error - added by weesan@cs.ucr.edu
		saveBestErrors();
	}
	bool popBestDrawOrder(void) {
		if (cBestDrawOrder.size() == 0) {
			return (false);
		} else {
			cBestDrawOrder.pop();
			return (true);
		}
	}
	void clearBestDrawOrder(void) {
		while (!cBestDrawOrder.empty()) {
			cBestDrawOrder.pop();
		}
	}
	// Return the best draw order of the symbol
	vector<int> &bestDrawOrder(void) {
		return (cBestDrawOrder.top().order());
	}
	// The weight is the best/least error for the best drawing order
	double bestDrawOrderWeight(void) const {
		return (cBestDrawOrder.top().weight());
	}
	int getIndex(int i) const {
		assert(i >= 0 && i < size());
		return (segindexmap[i]);
	}
	MySegment* getSegment(int i) const;
	Relationship* getRelation(int indexA,int indexB, bool *pbSwap = NULL) const;
	IntrinsicProp*  getIP(int indexA) const;
	// Added by weesan@cs.ucr.edu
	void sort(void);
	// Assigned draw order for the symbol
	void assignDrawOrder(const char *pzcAssignedOrder);
	void assignDrawOrder(const vector<int> &cAssignedOrder);
	// Restore the segment order to its original order
	void restoreOrigOrder(void) {
		segindexmap.clear();
		for (int i = 0; i < size(); i++) {
			segindexmap.push_back((*this)[i]->segNo());
		}
	}
	// Restore the segment order to its best order
	void restoreBestOrder(void) {
		segindexmap.clear();
		if (cBestDrawOrder.size()) {
			segindexmap = cBestDrawOrder.top().order();
		}
	}
	// Swap segments i and j
	void swapSegments(int i, int j) {
		int temp = segindexmap[i];
		segindexmap[i] = segindexmap[j];
		segindexmap[j] = temp;
	}
	// Compute local error for the symbol
	void computeLocalError(Definition *def);
	// Swap segments based on local error
	bool swapSegmentsBasedOnLocalError(void);
	// Save error metrics in seqential
	void saveError(double e) {
		cErrors.push_back(e);
	}
	// Clear errors
	void clearErrors(void) {
		cErrors.clear();
	}
	// Save a copy of the errors as best errors for debugging purpose
	void saveBestErrors(void) {
		cBestErrors = cErrors;
	}
	vector<double>&bestErrors(void) {
		return (cBestErrors);
	}
	void dumpErrors(void) {
		for (int i = 0; i < cErrors.size(); i++) {
			printf("%f ", cErrors[i]);
		}
		printf("\n");
	}
	int minX(void) const {
		return (iMinX);
	}
	int minY(void) const {
		return (iMinY);
	}
	int maxX(void) const {
		return (iMaxX);
	}
	int maxY(void) const {
		return (iMaxY);
	}
#ifdef DEF_CENTROID_WEIGHT
	Coord centroid(void) const {
		return (cCentroid);
	}
	void computeCentroid(void);
#endif
	void center(int *x, int *y);
	double totalLength(void) const {
		return (dTotalLength);
	}
	void flipSegment(int i);
	void flipSegments(void);
	bool xOverlapped(MySegment *pcSegment);
	bool mark(void) const {
		return (bMark);
	}
	void mark(bool _bMark) {
		bMark = _bMark;
	}

protected:
	void write(std::ostream &os) const;
	void read(std::istream &is);

public:
	vector <int> segindexmap;

private:
	priority_queue<BestDrawOrder> cBestDrawOrder;
	// Added by weesan@cs.ucr.edu
	vector<float> localError;
	vector<double> cErrors;		// The 6 error metrics without weights
	vector<double> cBestErrors;
	int iMinX, iMinY, iMaxX, iMaxY;
#ifdef DEF_CENTROID_WEIGHT
	Coord cCentroid;
#endif
	double dTotalLength;
	bool bMark;                 // A general purpose flag

	RelationTable RelTable;
};

// Added by weesan@cs.ucr.edu
class Symbols : public ScanDir, public vector<Symbol *> {
private:
	char pzcDir[_MAX_PATH + 1];
	int iRandom;

private:
	void resetMark(void) {
		for (int i = 0; i < size(); i++) {
			(*this)[i]->mark(false);
		}
	}
	void reverseMark(void) {
		for (int i = 0; i < size(); i++) {
			bool bMark = (*this)[i]->mark();
			(*this)[i]->mark(!bMark);
		}
	}

public:
	Symbols(void);
	Symbols(const char *_pzcDir, int _iRandom = 0);
	const char *dir(void) const {
		return (pzcDir);
	}
	int random(void) const {
		return (iRandom);
	}
	void name(char *pzcName);
	bool firstFile(void);
	bool nextFile(void);
	Symbol *firstSymbol(void);
	Symbol *nextSymbol(void);
	void compare(void);
	void display(void);
	void randomMark(int iCount);
	void dump(void) {
		for (int i = 0; i < size(); i++) {
			cout << *(*this)[i];
		}
	}
};

class TrainingSymbols : public map<string, Symbols, less<string> > {
private:
	void scanDir(const char *pzcDir, int iRandom, int iVerbose);

public:
	TrainingSymbols(const char *pzcUser, int iRandom, int iVerbose);
	void randomMark(int iCount);
	double aveSegmentCount(void);
	void dump(void);
};

#endif