// $Id: null_match.h,v 1.2 2005/09/16 04:37:11 weesan Exp $

/*
 * null_match.h - Do not perform any algorithm
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/22/2005
 */

#ifndef NULL_MATCH_H
#define NULL_MATCH_H

#include "match.h"

class NullMatch : public Match {
public:
	NullMatch(Symbol *pcSymbol, bool bTrainingMode = false) :
		Match("Null Match", pcSymbol, bTrainingMode) {
	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // NULL_MATCH_H