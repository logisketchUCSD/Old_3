// $Id: assigned_match.h,v 1.3 2005/09/16 04:37:11 weesan Exp $

/*
 * assigned_match.h - Assigned matching, for debugging purpose
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	7/12/2005
 */

#ifndef ASSIGNED_MATCH_H
#define ASSIGNED_MATCH_H

#include <string.h>
#include "match.h"
#include "debug.h"

class AssignedMatch : public Match {
private:
	char pzcAssignedOrder[256];

public:
	AssignedMatch(Symbol *pcSymbol, const char *_pzcAssignedOrder,
				  bool bTrainingMode = false) :
		Match("Assigned Matching", pcSymbol, bTrainingMode) {
		if (_pzcAssignedOrder) {
			snprintf(pzcAssignedOrder, sizeof(pzcAssignedOrder),
					 "%s", _pzcAssignedOrder);
		} else {
			pzcAssignedOrder[0] = NULL;
		}
	}
	const char *assignedOrder(void) const {
		return (pzcAssignedOrder);
	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // ASSIGNED_MATCH_H