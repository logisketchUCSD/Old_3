// $Id: options.cpp,v 1.2 2006/05/19 00:46:09 weesan Exp $

/*
 * options.cpp - Options from commandline.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/14/2005
 */

#include "options.h"

Options cOptions;

//////////////////////////////////

/*
 * For example: weesan:1:1-4:5-14:1-5
 *
 * Read all symbols from data/definitions.weesan and data/symbols.weesan,
 * make weesan as user 1, use matcher 1 to 4 (1 as stochastic, 2 as error-
 * driven, 3 as greedy and 4 as sort), traning examples start from 5 to 14,
 * run 1 to 5 times, and, generate result file with the format of
 * %02d%02d%02d%02d.txt.  The first file will have the file name like
 * 01050101.txt, and so on.
 */
void Options::parseValidation(const char *pzcOption) {
	validation(pzcOption);
	int n = sscanf(pzcOption, "%[^:]:%d:%d-%d:%d-%d:%d-%d",
				   pzcUser, &iUserNo,
				   &iFromMatcher, &iToMatcher,
				   &iFromTraining, &iToTraining,
				   &iFromRun, &iToRun);
	assert(n == 8);
}
