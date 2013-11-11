// $Id: assigned_match.cpp,v 1.3 2005/08/01 23:52:07 weesan Exp $

/*
 * assigned_match.cpp - Assigned matching, for debugging purpose
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	7/12/2005
 */

#include "assigned_match.h"

double AssignedMatch::findBestDrawOrder(Definition *pcDef) {
	// Assign the given order before computing global error
	symbol()->assignDrawOrder(assignedOrder());

	// Compute the global error
	double dBestMatch = computeMatch(pcDef);

	/*
	 * Remember it as the best draw order
	 * NOTE: Have to do this after computeMatch() since
	 * it also saves the errors
	 */
	symbol()->saveBestDrawOrder(dBestMatch);
	return (dBestMatch);
}
