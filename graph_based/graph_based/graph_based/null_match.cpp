// $Id: null_match.cpp,v 1.1 2005/08/23 01:14:08 weesan Exp $

/*
 * null_match.cpp - Do not perform any algorithm
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/22/2005
 */

#include "null_match.h"

double NullMatch::findBestDrawOrder(Definition *pcDef) {
	// Compute the global error
	double dBestMatch = computeMatch(pcDef);

	/*
	 * Remember it as the best draw order
	 * NOTE: Have to do this after computeMatch() since
	 * it also saves the errors
	 */
	symbol()->saveBestDrawOrder(dBestMatch);

	// Do not do any segment swapping
	return (dBestMatch);
}
