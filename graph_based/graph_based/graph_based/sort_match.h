// $Id: sort_match.h,v 1.4 2005/09/16 04:37:11 weesan Exp $

/*
 * sort_match.h - Sort matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/20/2005
 */

#ifndef SORT_MATCH_H
#define SORT_MATCH_H

#include "match.h"

class SortMatch : public Match {
protected:
	void restoreSymbolOrigOrder(void);

public:
	SortMatch(Symbol *pcSymbol, bool bTrainingMode = false);
	~SortMatch(void);
	double findBestDrawOrder(Definition *pcDef);
};

#endif // SORT_MATCH_H