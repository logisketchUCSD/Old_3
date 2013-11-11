// $Id: distance_match.h,v 1.1 2005/11/04 22:29:15 weesan Exp $

/*
 * distance_match.h - Distance matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/13/2005
 */

#ifndef DISTANCE_MATCH_H
#define DISTANCE_MATCH_H

#include "match.h"

class DistanceMatch : public Match {
public:
	DistanceMatch(Symbol *pcSymbol, bool bTrainingMode = false) :
		Match("Distance Match", pcSymbol, bTrainingMode) {
	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // DISTANCE_MATCH_H