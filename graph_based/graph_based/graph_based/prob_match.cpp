// $Id: prob_match.cpp,v 1.10 2006/12/27 22:25:25 weesan Exp $

/*
 * prob_match.cpp - Probabilistic matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#include "prob_match.h"
#include "random_match.h"
#include "options.h"

#define PM_NO_BETTER	150
#define PM_FAILSAFE		RM_ITR_DEFAULT // Worst case is to fallback to random matching

double ProbMatch::findBestDrawOrder(Definition *pcDef) {
	// Assume the first draw order is the best match
	double dBestMatch = computeMatch(pcDef);
	symbol()->saveBestDrawOrder(dBestMatch);

	/*
	 * If there are only 2 segments in a symbol,
	 * there is no point of swapping the order
	 */
	if (symbol()->size() <= 2) {
		return (dBestMatch);
	}

	int iNoBetterMax = cOptions.iteration() ? cOptions.iteration() : PM_NO_BETTER;
	int iNoBetter = 0;
	for (int i = 0; i < PM_FAILSAFE; i++) {
		// Compute symbol's local error against the given definition
		if (iNoBetter == 0) {
			symbol()->computeLocalError(pcDef);
		}

		// Swap 2 segments based on distribution of local errors
		if (symbol()->swapSegmentsBasedOnLocalError()) {
			incSwapCount();
		} else {
			// Fail to swap segments, probably because it gets closer to
			// be perfect.  So, break up early.
			break;
		}

		// Check if new draw order is better
		double dMatch = computeMatch(pcDef);
		if (dMatch > dBestMatch) {
			// Yes, remember the new order
			dBestMatch = dMatch;
			symbol()->saveBestDrawOrder(dBestMatch);
			iNoBetter = 0;
		} else {
			// No, swap it back
			symbol()->restoreBestOrder();
			iNoBetter++;
		}

		if (iNoBetter >= iNoBetterMax) {
			// Bail out early
			break;
		}
	}

	return (dBestMatch);
}
