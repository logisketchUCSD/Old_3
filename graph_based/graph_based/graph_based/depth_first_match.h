#ifndef DEPTH_FIRST_MATCH_H
#define DEPTH_FIRST_MATCH_H

#include "match.h"

class DepthFirstMatch : public Match {
public:
	DepthFirstMatch(Symbol *pcSymbol, bool bTrainingMode = false) :
		Match("Depth-first matching", pcSymbol, bTrainingMode) {
	}
	double findBestDrawOrder(Definition *pcDef);
};

#endif // DEPTH_FIRST_MATCH_H