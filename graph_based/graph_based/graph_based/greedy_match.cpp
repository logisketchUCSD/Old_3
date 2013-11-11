// $Id: greedy_match.cpp,v 1.8 2007/04/17 23:45:20 weesan Exp $

/*
 * greedy.cpp - Greedy/Systematic matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#include "greedy_match.h"
#include "definition.h"
#include "options.h"

#define GM_ITR_DEFAULT	 1
#define GM_TRAINING_DEV  0.01

double GreedyMatch::flipSegment(int i, Definition *pcDef, double dBestMatch) {
	// Flip the ith segment
	symbol()->flipSegment(i);
	// Have to re-process the symbol after flipping even one segment
	symbol()->Process();
	// Now compute the similarity score
	double dMatch = computeMatch(pcDef,
								 trainingMode() ? 
								 GM_TRAINING_DEV :
								 0.0);
	if (dMatch > dBestMatch) {
		// Yes, remember the new order
		dBestMatch = dMatch;
		symbol()->saveBestDrawOrder(dBestMatch);
	} else {
		// Otherwise, flip it back
		symbol()->flipSegment(i);
		symbol()->Process();
	}

	return (dBestMatch);
}

double GreedyMatch::findBestDrawOrderHelper(Definition *pcDef) {
	// Assume the first segment is the best fit
	double dBestMatch = computeMatch(pcDef);
	symbol()->saveBestDrawOrder(dBestMatch);
/*
	// Flip the first segment (0th segment) to see if it fits better
	if (trainingMode()) {
		dBestMatch = flipSegment(0, pcDef, dBestMatch);
	}
*/
	for (int i = 0; i < symbol()->size() - 1; i++) {
		for (int j = i + 1; j < symbol()->size(); j++) {
			// Swap the segments i and j
			symbol()->swapSegments(i, j);
			incSwapCount();

			// Check if new order is better
/*
			// The following performance better for symbols in
			// definitions.two_segments, but bad for others.
			// XXX: Need to investigate more.
			// Also note that if flipSegments() in Definition::AddTrainSymbol()
			// is commented out and the flip statements are uncommented here,
			// it gets higher accuracy rate, not sure why yet!
			double dMatch = computeMatch(pcDef,
										 trainingMode() ?
										 GM_TRAINING_DEV :
										 0.0);
*/
			double dMatch = computeMatch(pcDef);
			if (dMatch > dBestMatch) {
				// Yes, remember the new order
				dBestMatch = dMatch;
				symbol()->saveBestDrawOrder(dBestMatch);
			}
/*
			// Flip the segment that's being considered and recompute match
			if (trainingMode()) {
				dBestMatch = flipSegment(i, pcDef, dBestMatch);
			}
*/
			// Always swap it back for the next run
			symbol()->swapSegments(i, j);
		}
		/*
		 * So far, we have found the best ith segment.
		 * Calling restoreBestOrder() for all segments is a bit overkilled,
		 * but, all we care is to restore segments from 0 to ith.
		 */
		symbol()->restoreBestOrder();
	}

	return (dBestMatch);
}

double GreedyMatch::findBestDrawOrder(Definition *pcDef) {
	double dMatch = 0;
	int iItr = cOptions.iteration() ? cOptions.iteration() : GM_ITR_DEFAULT;
	for (int i = 0; i < iItr; i++) {
		dMatch = findBestDrawOrderHelper(pcDef);
	}
	return (dMatch);
}

#ifdef weesan
double GreedyMatch::findBestDrawOrder(Definition *pcDef) {
	assert(pcDef != NULL);

	// Assume the first draw order is the best match
	symbol()->saveBestDrawOrder(computeMatch(pcDef));

	// Take the first training sample from the definition
	Symbol cSample = *(*pcDef)[0];

	for (int i = 0; i < symbol()->size(); i++) {
		// Remember the segment being considered
		MySegment *pcSegment = cSample[i];
		int iBestSegment = 0;
		double dBestMatch = 0;

		for (int j = i; j < symbol()->size(); j++) {
			// Replace a segment from the training sample with the one
			// in the unknown symbol
			cSample[i] = (*symbol())[j];

			// Keep track of the number of swapping
			incSwapCount();

			// Have to re-process the training sample
			cSample.Process();

			double dMatch = pcDef->PercentMatch(&cSample, 0.05);
			if (dMatch > dBestMatch) {
				// Yes, remember the new order
				dBestMatch = dMatch;
				iBestSegment = j;
 				//printf("Hello 1\n");
			}

			// Flip the segment that's being considered and recompute match
			cSample.flipSegment(i);
			cSample.Process();
			dMatch = pcDef->PercentMatch(&cSample, 0.05);
 			if (dMatch > dBestMatch) {
				// Yes, remember the new order
				dBestMatch = dMatch;
				iBestSegment = j;
			} else {
				// Otherwise, flip it back
				cSample.flipSegment(i);
			}
		}

		// If the best segment is not the current segment, swap them
		if (i != iBestSegment) {
			symbol()->swapSegments(i, iBestSegment);
		}
		// Keep track of best segments so far
		symbol()->saveBestDrawOrder(dBestMatch);

		// Restore the segment of the training sample
		cSample[i] = pcSegment;
	}

	// Re-process symbol before returning because the segments of the symbol
	// might be flipped during the comparison above
	//symbol()->restoreBestOrder();
	symbol()->Process();

	return (symbol()->bestDrawOrderWeight());
}
#endif