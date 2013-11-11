// $Id: prob_match.h,v 1.3 2005/09/16 04:37:11 weesan Exp $

/*
 * prob_match.h - Probabilistic matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#ifndef PROB_MATCH_H
#define PROB_MATCH_H

#include "match.h"

class ProbMatch : public Match {
public:
	ProbMatch(Symbol *pcSymbol, bool bTrainingMode = false) :
		Match("Probabilistic matching", pcSymbol, bTrainingMode) {
	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // PROB_MATCH_H