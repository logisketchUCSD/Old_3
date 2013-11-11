// $Id: hybrid_match.h,v 1.2 2007/04/18 00:33:10 weesan Exp $

/*
 * hybrid_match.h - Hybrid matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	12/27/2006
 */

#ifndef HYBRID_MATCH
#define HYBRID_MATCH

#define HM_ITR_DEFAULT	720

#include "random_match.h"

class HybridMatch : public RandomMatch {
public:
	HybridMatch(Symbol *pcSymbol, bool bTrainingMode = false) :
		RandomMatch(pcSymbol, bTrainingMode, HM_ITR_DEFAULT) {	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // HYBRID_MATCH
