// $Id: match.h,v 1.18 2006/12/27 22:25:25 weesan Exp $

/*
 * match.h - Recognizer's base matching class
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include <queue>
#include "symbol.h"
#include "coord.h"

using namespace std;

// Forward declaration
class Definition;

class MatchEntry {
private:
	double dScore;
	int iDef;
	vector<int> cDrawOrder;
	vector<double> cErrors;
	Coord cCentroid;

public:
	MatchEntry(double dScore, int iDef,
			   vector<int>&cDrawOrder, vector<double>&cErrors,
			   Coord &cCentroid) :
		dScore(dScore),
		iDef(iDef),
		cDrawOrder(cDrawOrder),
		cErrors(cErrors),
		cCentroid(cCentroid) {
	}
	double score(void) const {
		return (dScore);
	}
	int def(void) const {
		return (iDef);
	}
	const char *defName(void) const;
	vector<int> &drawOrder(void) {
		return (cDrawOrder);
	}
	vector<double> &errors(void) {
		return (cErrors);
	}
	Coord centroid(void) const {
		return (cCentroid);
	}
	bool operator<(const MatchEntry &e) const {
		return (score() < e.score());
	}
};

class Match : public priority_queue<MatchEntry> {
public:
	enum Type {
		NULL_MATCH,
		STOCHASTIC,
		ERROR_DRIVEN,
		GREEDY,
		SORT,
		HYBRID,
		DISTANCE,
		ASSIGNED,
		MAX
	};

private:
	const char *pzcName;
	Symbol *pcSymbol;
	int iSwapCount;
	bool bTrainingMode;

protected:
	virtual void restoreSymbolOrigOrder(void) {
		symbol()->restoreOrigOrder();
	}
	double computeMatch(Definition *pcDef, double dDev = 0.0);

public:
	static Match *allocate(Type eType, Symbol *pcSymbol,
						   bool bTrainingMode = false,
						   const char *pzcAssignedOrder = NULL);
	static char *name(int iWhich);

public:
	Match(const char *pzcName, Symbol *pcSymbol, bool bTrainingMode) :
		pzcName(pzcName),
		pcSymbol(pcSymbol),
		iSwapCount(0),
		bTrainingMode(bTrainingMode) {
		// Clear the symbols's best draw order if any
		pcSymbol->clearBestDrawOrder();
	}
	virtual ~Match(void) {
	}
	const char *name(void) const {
		return (pzcName);
	}
	Symbol *symbol(void) const {
		return (pcSymbol);
	}
	int swapCount(void) const {
		return (iSwapCount);
	}
	void incSwapCount(void) {
		iSwapCount++;
	}
	bool trainingMode(void) const {
		return (bTrainingMode);
	}
	virtual double findBestDrawOrder(Definition *pcDef) = 0;
	void run(void);
};

#endif // MATCH_H