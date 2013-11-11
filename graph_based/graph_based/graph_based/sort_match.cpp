// $Id: sort_match.cpp,v 1.10 2005/09/16 04:37:11 weesan Exp $

/*
 * sort_match.cpp - Sort matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#include "sort_match.h"
#include "definition.h"

SortMatch::SortMatch(Symbol *pcSymbol, bool bTrainingMode) :
	Match("Sort Matching", pcSymbol, bTrainingMode) {
	static bool bDefsSorted = false;

	if (!bDefsSorted) {
		bDefsSorted = true;
		// Sort symbols in definitions
		for (int i = 0; i < cDefs.size(); i++) {
			cDefs[i]->sortSymbols();
		}
	}

	// Sort the symbols itself
	symbol()->sort();
}

SortMatch::~SortMatch(void) {
	// Restore symbols' draw order in definitions
	// Comment it out for now since SortMatch is not compatible with others
	// and it won't run together with others
	/*
	for (int i = 0; i < cDefs.size(); i++) {
		cDefs[i]->restoreOrigOrder();
	}
	*/
}

double SortMatch::findBestDrawOrder(Definition *pcDef) {
	// No comparison since the symbols are sorted
	double dBestMatch = computeMatch(pcDef);
	// Save it as the best order
	symbol()->saveBestDrawOrder(dBestMatch);
	return (dBestMatch);
}

// No need to restore symbol draw order while comparing against defintions
void SortMatch::restoreSymbolOrigOrder(void) {
}
