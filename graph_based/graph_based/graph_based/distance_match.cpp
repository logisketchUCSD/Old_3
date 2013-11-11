// $Id: distance_match.cpp,v 1.2 2006/12/20 06:21:52 weesan Exp $

/*
 * distance_match.h - Distance matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/13/2005
 */

#include <assert.h>
#include "distance_match.h"
#include "definition.h"
#include "distance.h"

double DistanceMatch::findBestDrawOrder(Definition *pcDef) {
	assert(pcDef != NULL && pcDef->size() != 0);

	// Find a good reference among the symbols in a definition
	int iGoodRef = 0;
	for (int i = 0; i < pcDef->size(); i++) {
		if ((*pcDef)[i]->size() == pcDef->NumofSegments()) {
			iGoodRef = i;
			break;
		}
	}

	Distance dDistance(*symbol(), *(*pcDef)[iGoodRef]);

	symbol()->assignDrawOrder(dDistance.drawOrder());

	// Compute the best match score
	double dBestMatch = computeMatch(pcDef);
	symbol()->saveBestDrawOrder(dBestMatch);

	return (dBestMatch);
}
