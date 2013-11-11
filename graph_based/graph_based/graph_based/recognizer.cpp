// $Id: recognizer.cpp,v 1.37 2006/12/27 22:25:25 weesan Exp $

/*
 * recognizer.cpp - A graph-based recognizer (based on glue.[cpp|h])
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/19/2005
 */

#include <assert.h>
#include "recognizer.h"
#include "definition.h"
#include "glue.h"
#include "debug.h"
#include "recog_viewer.h"
#include "result.h"
#include "stats.h"
#include "confusion_matrix.h"
#include "options.h"

//#define RECOGNIZER_PERCENT_START	0.01
//#define RECOGNIZER_PERCENT_INC		0.01
#define RECOGNIZER_PERCENT_START	0.05
#define RECOGNIZER_PERCENT_INC		0.05

using namespace std;

//////////////////////////////////

void Recognizer::displaySymbolGraph(const char *pzcName, Symbol *pcSymbol) {
	printf("Test symbol ==> %s\n", pzcName);
	for (int i = 0; i < pcSymbol->size(); i++) {
		for (int j = i; j < pcSymbol->size(); j++) {
			if (i == j) {
				IntrinsicProp *pcProp = pcSymbol->getIP(i);
				printf("\t   %d-%c: ", i, pcProp->SType() == LINE ? 'L' : 'A');
				printf("RL=%f\n",
					   pcProp->RelLen());
			} else {
				Relationship *pcRel = pcSymbol->getRelation(i, j);
				printf("\t   %d-%d: ", i, j);
				printf("IN=%d, IA=%.1f, IL=%.02f,%.02f\n",
					   pcRel->NumofIntersections(),
					   Rad2Deg(pcRel->IntAngle()),
					   pcRel->IntLocA1(), pcRel->IntLocB1());
			}
		}
	}
	printf("\n");
}

void Recognizer::displayErrors(vector<double> &cErrors) {
	double dTotalError = 0;
	printf("\t  (");
	for (int i = 0; i < cErrors.size(); i++) {
		printf("%.05f ", cErrors[i]);
		switch (i) {
		case 0:
			dTotalError += cErrors[i] * DEF_PRIM_TYPE_WEIGHT;
			break;
		case 1:
			dTotalError += cErrors[i] * DEF_PRIM_COUNT_WEIGHT;
			break;
		case 2:
			dTotalError += cErrors[i] * DEF_REL_LEN_WEIGHT;
			break;
		case 3:
			dTotalError += cErrors[i] * DEF_INT_NO_WEIGHT;
			break;
		case 4:
			dTotalError += cErrors[i] * DEF_INT_ANG_WEIGHT;
			break;
		case 5:
			dTotalError += cErrors[i] * DEF_INT_LOC_WEIGHT;
			break;
		default:
			break;
		}
	}
	printf("= %.05f)\n", dTotalError);
}

void Recognizer::displayDefinitionGraph(Symbol *pcSymbol, MatchEntry &e) {
	Definition *pcDef = cDefs[e.def()];
	for (int i = 0; i < pcDef->NumofSegments(); i++) {
		for (int j = i; j < pcDef->NumofSegments(); j++) {
			if (i == j) {
				IntrinsicProp *pcProp = pcDef->getIP(i);
				printf("\t   %d-%c: ", i, pcProp->SType() == LINE ? 'L' : 'A');
				printf("RL=%f(%f)\n",
					   pcProp->RelLen(), pcProp->RelLen_stdev());
			} else {
				Relationship *pcRel = pcDef->getRelation(i, j);
				printf("\t   %d-%d: ", i, j);
				printf("IN=%d, IA=%.1f (%.1f), IL=%.02f,%.02f (%.02f,%.02f)\n",
					   pcRel->NumofIntersections(),
					   Rad2Deg(pcRel->IntAngle()),
					   Rad2Deg(pcRel->IntAngle_stdev()),
					   pcRel->IntLocA1(), pcRel->IntLocB1(),
					   pcRel->IntLocA1_stdev(), pcRel->IntLocB1_stdev());
			}
		}
	}
}

void Recognizer::displayResult(Symbol *pcSymbol, const char *pzcSymbol,
							   int iVerbose, bool bMatch,
							   MatchEntry &e, int iRank) {
	printf("\t%c %f [%s - %s]",
		   bMatch ? '>' : ' ',
		   e.score(),
		   pzcSymbol, 
		   e.defName());

	// Display best draw order
	if (iVerbose >= 1) {
		for (int i = 0; i < e.drawOrder().size(); i++) {
			char pzcDrawOrder[16];

			if (e.drawOrder()[i] < 0) {
				snprintf(pzcDrawOrder, sizeof(pzcDrawOrder), "?");
			} else {
				snprintf(pzcDrawOrder, sizeof(pzcDrawOrder),
						 "%d", e.drawOrder()[i]);
			}

			if (i == 0) {
				printf(" %s", pzcDrawOrder);
			} else {
				printf("-%s", pzcDrawOrder);
			}
		}
	}

	// Display rank
	if (iRank > 0) {
		printf(" (%d)\n", iRank);
	} else {
		printf("\n");
	}

	// Display local errors
	if (iVerbose >= 2) {
		displayErrors(e.errors());
	}

	// Display relationship matrix
	if (iVerbose >= 3) {
		displayDefinitionGraph(pcSymbol, e);
	}
}

bool Recognizer::recognize(Symbol *pcSymbol,
						   const char *pzcName,
						   Match::Type eMatch, int iBestN,
						   const char *pzcAssignedOrder,
						   int iVerbose,
   						   char *pzcMatchedName) {
	bool bRet = false;

	if (iVerbose >= 3) {
		//cout << *pcSymbol;
		displaySymbolGraph(pzcName, pcSymbol);
	}

	Match *pcMatch = Match::allocate(eMatch, pcSymbol, false, pzcAssignedOrder);
	assert(pcMatch != NULL);
	pcMatch->run();

	if (iVerbose >= 1) {
		printf("%s (%d):\n", pcMatch->name(), pcMatch->swapCount());
	}

	int i = 0;
	bool bFound = false;
	for (i = 0; i < iBestN && !pcMatch->empty(); i++) {
		MatchEntry &e = pcMatch->top();

		// Keep track of matched definition's name if provided buffer to do so
		if (i == 0 && pzcMatchedName) {
			strcpy(pzcMatchedName, e.defName());
			//printf("%s: %d\n", e.defName(), pcMatch->swapCount());
		}

		bool bMatch = (strcmp(pzcName, e.defName()) == 0);
		if (bMatch) {
			bFound = true;
			// Even the symbol has been classified, we keep going till 
			// we find the N best one

			// If it's first choice, we will return true, otherwise, false
			if (i == 0) {
				bRet = true;
			}
		}

		if (iVerbose >= 1) {
			displayResult(pcSymbol, pzcName, iVerbose, bMatch, e);
		}

		pcMatch->pop();
	}

	// If N best one is not our answer, we keep going
	if (!bFound) {
		while (!pcMatch->empty()) {
			MatchEntry &e = pcMatch->top();
			bool bMatch = (strcmp(pzcName, e.defName()) == 0);
			if (bMatch) {
				if (iVerbose >= 1) {
					printf("\t  ...\n");
					displayResult(pcSymbol, pzcName,
								  iVerbose, true, e, i + 1);
				}
				break;
			}
			pcMatch->pop();
			i++;
		}
	}

	if (iVerbose >= 1) {
		printf("\n");
	}

	// Free the matching mechanism
	delete pcMatch;

	//return (bRet);

	// Return true when it matches the first N best
	return (bFound);
}

void Recognizer::recognize(const char *pzcSymbolDir,
						   Match::Type eMatch, int iBestN,
						   const char *pzcAssignedOrder,
						   int iVerbose,
						   int iRandom) {
	assert(pzcSymbolDir != NULL);

	// If assiged order is given, it will override -m option
	if (pzcAssignedOrder && *pzcAssignedOrder) {
		eMatch = Match::ASSIGNED;
	}

	if (cDefs.size() == 0) {
		fprintf(stderr, "Failed to match symbols: no definitions\n");
		return;
	}

	if (iVerbose == 0) {
		printf("%s ...\n", Match::name(eMatch));
	}

	Result cResult;
	ConfusionMatrix cConfusionMatrix;
	Symbols cSymbols(pzcSymbolDir, iRandom);
	for (bool bRet = cSymbols.firstFile(); bRet; bRet = cSymbols.nextFile()) {
		for (Symbol *pcSymbol = cSymbols.firstSymbol();
			 pcSymbol;
			 pcSymbol = cSymbols.nextSymbol()) {
			cResult.incTotal();
			// Retrieve the name of the symbol without the extension
			char pzcName[_MAX_PATH + 1], pzcMatchedName[_MAX_PATH + 1];
			pzcName[0] = pzcMatchedName[0] = 0;
			cSymbols.name(pzcName);

			cResult.start();
			bool bFound = recognize(pcSymbol, pzcName, eMatch, iBestN,
								    pzcAssignedOrder, iVerbose,
									pzcMatchedName);
			cResult.finish();

			if (bFound) {
				cResult.incCorrect();
			}

			cConfusionMatrix[pzcName][pzcMatchedName]++;
			//printf("=> %s, %s\n", pzcName, pzcMatchedName);

			// Free the symbol
			delete pcSymbol;
		}
	}

	printf("\n"
		   "Accuracy = %.02f%%, Average time taken per symbol = %.02fms\n"
		   "\n",
		   cResult.accuracy(), cResult.average());

	cConfusionMatrix.dump(stdout);
}

void Recognizer::recognize(Symbol *pcSymbol,
						   const char *pzcName,
						   Match::Type eMatch) {
	Match *pcMatch = Match::allocate(eMatch, pcSymbol);
	assert(pcMatch != NULL);
	pcMatch->run();
#ifdef HAVE_FLTK
	RecogViewer cViewer(pcSymbol, pcMatch);
	cViewer.run();
#else
	fprintf(stderr, "Warning: no FLTK!\n");
#endif
	delete pcMatch;
}

void Recognizer::recognize(TrainingSymbols &cTrainingSymbols,
						   int iBestN,
						   int iUserNo,
						   int iFromTraining, int iToTraining,
						   int iFromMatcher, int iToMatcher,
						   int iFromRun, int iToRun,
						   int iVerbose) {
	// A summary file
	char pzcSummary[_MAX_PATH];
	snprintf(pzcSummary, sizeof(pzcSummary),
			 "user_study\\%02d\\%02d_summary.txt", iUserNo, iUserNo);
	// XXX
	//FILE *sp = fopen(pzcSummary, "w");
	FILE *sp = fopen(pzcSummary, "a");
	if (!sp) {
		fprintf(stderr,
				"Failed to open %s: %s\n", pzcSummary, strerror(errno));
		return;
	}
	// Annotate the parameters used
	fprintf(sp, "Parameters: -I%d -r%d -b%d\n\n",
			cOptions.iteration(), cOptions.random(), cOptions.bestN());

	for (int iMatcher = iFromMatcher; iMatcher <= iToMatcher; iMatcher++) {
		ConfusionMatrix cAveConfusionMatrix;
		Stats cAveAveAccurary, cAveAveElapsedTime;
		for (int iTraining = iFromTraining; iTraining <= iToTraining;iTraining++) {
			Stats cAveAccuracy, cAveElapsedTime;
			for (int iRun = iFromRun; iRun <= iToRun; iRun++) {
				char pzcOutFile[_MAX_PATH];
				snprintf(pzcOutFile, sizeof(pzcOutFile),
						 "user_study\\%02d\\%02d%02d%02d%02d.txt",
						 iUserNo,
						 iUserNo, iMatcher, iTraining, iRun);
				printf("==> %s\n", pzcOutFile);
				// Open the output file
				FILE *fp = fopen(pzcOutFile, "w");
				if (!fp) {
					fprintf(stderr,
							"Failed to open %s: %s\n",
							pzcOutFile, strerror(errno));
					break;
				}
				// Randomly picks examples as training
				cTrainingSymbols.randomMark(iTraining);
				// Re-learn the definitions
				cDefs.clear();
				cDefs.readTrainingSymbols(cTrainingSymbols, iVerbose);
				// Keep track of the result
				Result cResult;
				// Confusion matrix
				ConfusionMatrix cConfusionMatrix;
				// Loop thru all other examples
				for (TrainingSymbols::iterator itr = cTrainingSymbols.begin();
					 itr != cTrainingSymbols.end();
					 itr++) {
					const char *pzcName = itr->first.c_str();
					char pzcMatchedName[_MAX_PATH + 1];
					fprintf(fp, "%10s: ", pzcName);
					Symbols &cSymbols = itr->second;
					for (int i = 0; i < cSymbols.size(); i++) {
						Symbol *pcSymbol = cSymbols[i];
						if (pcSymbol->mark()) {
							fprintf(fp, "T");
						} else {
							fprintf(fp, ".");
							cResult.incTotal();
							pzcMatchedName[0] = 0;
							cResult.start();
							bool bFound = recognize(pcSymbol,
													pzcName,
													(Match::Type)iMatcher,
													iBestN,
												    NULL,
													1,
													pzcMatchedName);

							cResult.finish();

							if (bFound) {
								cResult.incCorrect();
							}
	
							cConfusionMatrix[pzcName][pzcMatchedName]++;
							cAveConfusionMatrix[pzcName][pzcMatchedName]++;
						}
					}
					fprintf(fp, "\n");
				}

				cAveAccuracy.add(cResult.accuracy());
				cAveElapsedTime.add(cResult.average());
				fprintf(fp,
						"\n"
					   "Accuracy (%d/%d) = %.02f%%, Average time taken per symbol = %.02fms\n"
					   "\n",
					   cResult.correct(), cResult.total(),
					   cResult.accuracy(), cResult.average());
				fflush(sp);
				cConfusionMatrix.dump(fp);
				// Close the output file
				fclose(fp);
			}
			// Print out average info
			fprintf(sp, "%02d %2d %2d %f %f\n",
					iUserNo, iMatcher, iTraining,
					cAveAccuracy.mean(), cAveElapsedTime.mean());
			cAveAveAccurary.add(cAveAccuracy.mean());
			cAveAveElapsedTime.add(cAveElapsedTime.mean());
		}
		fprintf(sp, "    Ave(%d): %f %f\n",
				iMatcher, cAveAveAccurary.mean(), cAveAveElapsedTime.mean());
		fprintf(sp, "\n");
		cAveConfusionMatrix.dump(sp);
		fprintf(sp, "\n");
	}

	// Close the summary file
	fclose(sp);
}

void Recognizer::wrapper(const char *pzcSymbolDir) {
	assert(pzcSymbolDir != NULL);

	if (cDefs.size() == 0) {
		fprintf(stderr, "Failed to match symbols: no definitions\n");
		return;
	}

	Symbols cSymbols(pzcSymbolDir);
	for (bool bRet = cSymbols.firstFile(); bRet; bRet = cSymbols.nextFile()) {
		for (Symbol *pcSymbol = cSymbols.firstSymbol();
			 pcSymbol;
			 pcSymbol = cSymbols.nextSymbol()) {
			double dBestMatch = 0.0;
			// Retrieve the name of the symbol without the extension
			char pzcName[_MAX_PATH + 1];
			cSymbols.name(pzcName);
			// Start brute-force comparison
			for (double i = RECOGNIZER_PERCENT_START; i < 1.0; i += RECOGNIZER_PERCENT_INC) {
				for (double j = RECOGNIZER_PERCENT_START; j < 1.0; j += RECOGNIZER_PERCENT_INC) {
					for (double k = RECOGNIZER_PERCENT_START; k < 1.0; k += RECOGNIZER_PERCENT_INC) {
						for (double l = RECOGNIZER_PERCENT_START; l < 1.0; l += RECOGNIZER_PERCENT_INC) {
							for (double m = RECOGNIZER_PERCENT_START; m < 1.0; m += RECOGNIZER_PERCENT_INC) {
								double n = 1 - i - j - k - l - m;
								if (n <= EPSILON) {
									break;
								}
								for (int d = 0; d < cDefs.size(); d++) {
									double dMatch = cDefs[d]->PercentMatch(pcSymbol, false, i, j, k, l, m, n);
									if (dMatch > dBestMatch && stricmp(pzcName, cDefs[d]->name()) == 0) {
									//if (dMatch > dBestMatch) {
										dBestMatch = dMatch;
										printf("%.02f %.02f %.02f %.02f %.02f %.02f : %s (%.06f)\n",
											   i, j, k, l, m, n, cDefs[d]->name(), dMatch);
										fflush(stdout);
									}
								}
							}
						}
					}
				}
			}
			delete pcSymbol;
		}
	}
}
