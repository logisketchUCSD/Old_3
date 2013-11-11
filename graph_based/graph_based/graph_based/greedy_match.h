// $Id: greedy_match.h,v 1.4 2007/04/17 23:45:20 weesan Exp $

/*
 * greedy_match.h - Greedy/Systematic matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#ifndef GREEDY_MATCH_H
#define GREEDY_MATCH_H

#include "match.h"

class GreedyMatch : public Match {
private:
	double flipSegment(int i, Definition *pcDef, double dBestMatch);
	double findBestDrawOrderHelper(Definition *pcDef);

public:
	GreedyMatch(Symbol *pcSymbol, bool bTrainingMode = false) :
		Match("Greedy Match", pcSymbol, bTrainingMode) {
	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // GREEDY_MATCH_H