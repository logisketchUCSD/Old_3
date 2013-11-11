// $Id: random_match.cpp,v 1.10 2006/12/27 22:25:25 weesan Exp $

/*
 * random_match.cpp - Random matching (based on Burak's glue code)
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#include "random_match.h"
#include "definition.h"
#include "options.h"

double RandomMatch::findBestDrawOrder(Definition *pcDef) {
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

	int iMaxLoop = cOptions.iteration() ? cOptions.iteration() : maxItr();
	for (int i = 0; i < iMaxLoop; i++) {
		// Randomly swap the segments
		symbol()->Shuffle_Segments(pcDef->NumofSegments());
		// Keep track the number of swapping
		incSwapCount();
		// Check if new draw order is any better
		double dMatch = computeMatch(pcDef);
		if (dMatch > dBestMatch) {
			// Yes, keep it
			dBestMatch = dMatch;
			symbol()->saveBestDrawOrder(dBestMatch);
		}
	}

	return (dBestMatch);
}
