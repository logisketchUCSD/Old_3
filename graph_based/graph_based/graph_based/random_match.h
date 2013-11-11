// $Id: random_match.h,v 1.5 2006/12/27 22:25:25 weesan Exp $

/*
 * random_match.h - Random matching (based on Burak's glue code)
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#ifndef RANDOM_MATCH_H
#define RANDOM_MATCH_H

#include "match.h"

#define RM_ITR_DEFAULT    300

class RandomMatch : public Match {
private:
	int iMaxItr;

public:
	RandomMatch(Symbol *pcSymbol, bool bTrainingMode = false,
				int iMaxItr = RM_ITR_DEFAULT) :
		Match("Random matching", pcSymbol, bTrainingMode),
		iMaxItr(iMaxItr) {
	}
	int maxItr(void) const {
		return (iMaxItr);
	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // RANDOM_MATCH_H