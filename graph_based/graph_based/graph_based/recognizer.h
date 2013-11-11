// $Id: recognizer.h,v 1.18 2006/12/23 08:37:40 weesan Exp $

/*
 * recognizer.h - A graph-based recognizer (based on glue.[cpp|h])
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/19/2005
 */

#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <vector>
// NOTE: When using map, the following #pragma is needed to get it to work
// although there are still some warnings.
// See http://support.microsoft.com/default.aspx?scid=kb;EN-US;q167355 for details
#pragma warning(disable:4786)
#include <map>
#include <string>
#include "stdafx.h"
#include "ink.h"
#include "match.h"
#include "symbol.h"

using namespace std;

#define RECOGNIZER_BEST_N_MATCH		1

class Recognizer {
private:
	void displaySymbolGraph(const char *pzcName, Symbol *pcSymbol);
	void displayErrors(vector<double> &cErrors);
	void displayDefinitionGraph(Symbol *pcSymbol, MatchEntry &e);
	void displayResult(Symbol *pcSymbol, const char *pzcSymbol,
					   int iVerbose, bool bMatch,
					   MatchEntry &e, int iRank = 0);

public:
	bool recognize(Symbol *pcSymbol,
				   const char *pzcName,
				   Match::Type eMatch,
				   int iBestN,
				   const char *pzcAssignedOrder = NULL,
				   int iVerbose = 1,
				   char *pzcMatchedName = NULL);
	void recognize(const char *pzcSymbolDir,
				   Match::Type eMatch,
				   int iBestN,
				   const char *pzcAssignedOrder,
				   int iVerbose,
				   int iRandom = 0);
	void recognize(Symbol *pcSymbol,
				   const char *pzcName,
				   Match::Type eMatch);
	void recognize(TrainingSymbols &cTrainingSymbols,
				   int iBestN,
				   int iUserNo,
				   int iFromTraining, int iToTraining,
				   int iFromMatcher, int iToMatcher,
				   int iFromRun, int iToRun,
				   int iVerbose);
	void wrapper(const char *pzcSymbolDir);
};

#endif // RECOGNIZER_H