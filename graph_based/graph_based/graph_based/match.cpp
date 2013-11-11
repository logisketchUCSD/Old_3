// $Id: match.cpp,v 1.24 2006/12/27 22:25:25 weesan Exp $

/*
 * match.cpp - Recognizer's base matching class
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#include "match.h"
#include "definition.h"
#include "random_match.h"
#include "sort_match.h"
#include "prob_match.h"
#include "greedy_match.h"
#include "assigned_match.h"
#include "null_match.h"
#include "distance_match.h"
#include "hybrid_match.h"
#include "symbol.h"

const char *MatchEntry::defName(void) const {
	return (cDefs[def()]->name());
}

//////////////////////////////////

double Match::computeMatch(Definition *pcDef, double dDev) {
	if (trainingMode()) {
		return (pcDef->PercentMatch(symbol(), trainingMode(), dDev));
	} else {
		return (pcDef->PercentMatch(symbol(), trainingMode(), dDev,
				DEF_PRIM_TYPE_WEIGHT,
				DEF_PRIM_COUNT_WEIGHT,
				DEF_REL_LEN_WEIGHT,
				DEF_INT_NO_WEIGHT,
				DEF_INT_ANG_WEIGHT,
				DEF_INT_LOC_WEIGHT
#if defined(DEF_CONN_WEIGHT) || defined(DEF_CENTROID_WEIGHT) || defined(DEF_LOC_WEIGHT)
				, 0
#endif
				));
	}
}

void Match::run(void) {
	// Compare against each definition
	//for (iCurrentDef = 0; iCurrentDef < cDefs.size(); iCurrentDef++) {
	for (int i = 0; i < cDefs.size(); i++) {
		/*
		 * As a fair comparison, the segment order of the symbol is restored
		 * before comparing against each new definition
		 */
		restoreSymbolOrigOrder();
		symbol()->clearBestDrawOrder();
	 
		// Reset swap count
		//iSwapCount = 0;

		// Find the best draw order for a given definition
		double dMatch = findBestDrawOrder(cDefs[i]);
		push(MatchEntry(dMatch, i,
					    symbol()->bestDrawOrder(), symbol()->bestErrors(),
						cDefs[i]->centroid()));
	}
}

Match *Match::allocate(Type eType, Symbol *pcSymbol,
					   bool bTrainingMode,
					   const char *pzcAssignedOrder) {
	Match *pcMatch = NULL;

	switch (eType) {
	case NULL_MATCH:
		pcMatch = new NullMatch(pcSymbol, bTrainingMode);
		break;
	case STOCHASTIC:
		pcMatch = new RandomMatch(pcSymbol, bTrainingMode);
		break;
	case ERROR_DRIVEN:
		pcMatch = new ProbMatch(pcSymbol, bTrainingMode);
		break;
	case GREEDY:
		pcMatch = new GreedyMatch(pcSymbol, bTrainingMode);
		break;
	case SORT:
		pcMatch = new SortMatch(pcSymbol, bTrainingMode);
		break;
	case HYBRID:
		pcMatch = new HybridMatch(pcSymbol, bTrainingMode);
		break;
	case DISTANCE:
		pcMatch = new DistanceMatch(pcSymbol, bTrainingMode);
		break;
	case ASSIGNED:
		pcMatch = new AssignedMatch(pcSymbol, pzcAssignedOrder, bTrainingMode);
		break;
	default:
		break;
	}

	return (pcMatch);
}

char *Match::name(int iWhich) {
	static char pzcName[][32] = {
		"Null Matching",
		"Stochastic Matching",
		"Error-driven Matching",
		"Greedy Matching",
		"Sort Matching",
		"Hybrid Matching",
		"Distance Matching",
		"Assigned Matching"
	};

	if (iWhich >= 0 && iWhich < MAX) {
		return (pzcName[iWhich]);
	} else {
		return ("Unknown matching");
	}
}
