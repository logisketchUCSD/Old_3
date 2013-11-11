// $Id: ysmain.cpp,v 1.39 2007/03/10 05:08:05 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2005
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/

#include "stdafx.h"
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "GetOpt.h"
#include "recognizer.h"
#include "definition.h"
#include "options.h"
#include "extern.h"
#include "debug.h"

/* in link options, change subsystem from "windows" to "console" */

#include "definition.h"

#define USAGE()	{                       \
	fprintf(stderr, pzcUsage, argv[0]); \
	exit(1);                            \
}

static const char pzcUsage[] = 
"\n"
"Usage: %s [options]\n"
"where options are as below:\n"
"\t-h\tHelp\n"
"\t-l\tDefinitions/Library directory\n"
"\t-s\tSymbols directory\n"
"\t-m\tMatching mechanism (0:Null 1:Stochastic 2:Error-driven 3:Greedy 4:Sort 5:Hybrid 6:Distance 7:Assigned)\n"
"\t-b\tBest N matches.  Default is 1\n"
"\t-v\tVerbose level\n"
"\t-w\tA wrapper to find the best weight\n"
"\t-a\tAssign symbol order, eg. 0-1-2-3. Imply -m5\n"
"\t-c\tConvert raw definitions to processed data\n"
"\t-o\tOutput directory\n"
"\t-i\tTraining order (0:Vanilla 1:Increment 2:Feedback 3:Best-N 4:Distance)\n"
"\t-r\tRandomly shuffle segments in definition symbols.  Default 0\n"
"\t-t\tTraining mode. Eg. -tweesan:5+10 means user=weesan, 5 test and 10 training samples\n"
"\t-n\tValue N for the Best-N approach. ie. use it with -i3\n"
"\t-p\tPlot the relation graph for the definitions.  Use it with -o\n"
"\t-e\tSpeed threshold.  Default 0.25\n"
"\t-k\tRun k-fold validation. Eg. -k'weesan:1:1-4:5-14:1-5'\n"
"\t-I\tThe number of iterations.  Specific to different matchers.  Use it with care\n"
"\n";

// Find the window named 'pzcName' and send keystroke F5,
// which is good for browser refreshing
static void findWindow(const char *pzcName, HWND hParent = NULL) {
	// Get the first window
	HWND hWin = FindWindowEx(hParent, NULL, NULL, NULL);
	while (hWin) {
		char pzcBuffer[512];
		GetWindowText(hWin, pzcBuffer, sizeof(pzcBuffer));
		if (pzcName == NULL || strstr(pzcBuffer, pzcName)) {
			//printf("Found: [%s]\n", pzcBuffer);
			PostMessage(hWin, WM_KEYDOWN, VK_F5, 0);
			// Call itself recursively to make sure all child windows
			// get the message
			findWindow(NULL, hWin);
		}
		// Get next window
		hWin = FindWindowEx(hParent, hWin, NULL, NULL);
	}
}

int main(int argc,char *argv[]) {

	// Added by weesan@cs.ucr.edu
	int iOpt;
	GetOpt cGetOpt(argc, argv, "hl:s:m:b:vwa:co:i:r:t:n:pe:k:I:");

	cOptions.bestN(RECOGNIZER_BEST_N_MATCH);

	while ((iOpt = cGetOpt()) != EOF) {
		switch (iOpt) {
		case 'l': {
			char pzcBuffer[_MAX_PATH];
			snprintf(pzcBuffer, sizeof(pzcBuffer), "%s\\%s",
					 DEF_DIR_PREFIX, cGetOpt.optarg);
			cOptions.defDir(pzcBuffer);
			break;
		}
		case 's': {
			char pzcBuffer[_MAX_PATH];
			snprintf(pzcBuffer, sizeof(pzcBuffer), "%s\\%s",
					 DEF_DIR_PREFIX, cGetOpt.optarg);
			cOptions.symbolDir(pzcBuffer);
			break;
		}
		case 'm':
			cOptions.match((Match::Type)atoi(cGetOpt.optarg));
			break;
		case 'b':
			cOptions.bestN(atoi(cGetOpt.optarg));
			break;
		case 'v':
			cOptions.verbose(cOptions.verbose() + 1);
			break;
		case 'w':
			cOptions.wrapper(true);
			break;
		case 'a':
			cOptions.assignOrder(cGetOpt.optarg);
			break;
		case 'c':
			cOptions.convert(true);
			break;
		case 'o':
			cOptions.outDir(cGetOpt.optarg);
			break;
		case 'i':
			cOptions.defsType((Definitions::Type)atoi(cGetOpt.optarg));
			break;
		case 'r':
			cOptions.random(atoi(cGetOpt.optarg));
			break;
		case 't':
			cOptions.parseTrainingMode(cGetOpt.optarg);
			break;
		case 'n':
			cOptions.valueN(atoi(cGetOpt.optarg));
			break;
		case 'p':
			cOptions.plot(true);
			break;
		case 'e':
			ParamSpeedThresh = atof(cGetOpt.optarg);
			break;
		case 'k':
			cOptions.parseValidation(cGetOpt.optarg);
			break;
		case 'I':
			cOptions.iteration(atoi(cGetOpt.optarg));
			break;
		case 'h':
			// Fall thru
		default:
			USAGE();
			break;
		}
	}

	argc -= cGetOpt.optind;
	argv += cGetOpt.optind;

	// XXX: Don't forget to change it back to random :)
	// Random
	unsigned int uiNow = time(NULL);
	// Time that fails: 1121923046  1121923196
	//unsigned int uiNow = 1121923046;
	// Time that succeeds: 1121923137  1121923174
	//unsigned int uiNow = 1121923174;
	srand(uiNow);

	// Check if both -c and -o are specified
	if (cOptions.convert() && cOptions.outDir() == NULL) {
		fprintf(stderr, "Error: option -p and -o are needed together\n");
		exit(1);
	}

	// Check if both -p and -o are specified
	if (cOptions.plot() && cOptions.outDir() == NULL) {
		fprintf(stderr, "Error: option -p and -o are needed together\n");
		exit(1);
	}

	// Check if both -n and -i3 are specified
	if (cOptions.valueN() != 0 && cOptions.defsType() != Definitions::BEST_N) {
		fprintf(stderr, "Error: option -i%d is needed with -n\n",
				Definitions::BEST_N);
		exit(1);
	}

	// After validation, terminate the program
	if (cOptions.validation()) {
		TrainingSymbols cTrainingSymbols(cOptions.user(), cOptions.random(),
										 cOptions.verbose());
		Recognizer cRecognizer;
		cRecognizer.recognize(cTrainingSymbols,
							  cOptions.bestN(),
							  cOptions.userNo(),
							  cOptions.fromTraining(), cOptions.toTraining(),
							  cOptions.fromMatcher(), cOptions.toMatcher(),
							  cOptions.fromRun(), cOptions.toRun(),
							  cOptions.verbose());

		exit(0);
	}

	if (!cOptions.trainingMode()) {
		// Pre-read definition symbols
		switch (cOptions.defsType()) {
		case Definitions::INCREMENT:
			cDefs.readIncrement(cOptions.defDir(), cOptions.match(),
								cOptions.random());
			break;
		case Definitions::FEEDBACK:
			cDefs.readIncrement(cOptions.defDir(), cOptions.match(),
								cOptions.random());
			cDefs.feedback(cOptions.match());
			break;
		case Definitions::BEST_N:
			cDefs.readIncrement(cOptions.defDir(), cOptions.match(),
								cOptions.random());
			cDefs.bestN(cOptions.match(), cOptions.valueN());
			break;
		case Definitions::DISTANCE:
			cDefs.readIncrement(cOptions.defDir(), cOptions.match(),
								cOptions.random(), true);
			break;
		case Definitions::VANILLA:
			// Fall thru
		default:
			cDefs.read(cOptions.defDir());
			break;
		}

		if (cOptions.verbose() >= 3) {
			// Commented out for now
			//cDefs.dumpIncStdDev();
			cDefs.dumpGraph();
		}

		if (cOptions.verbose() >= 4) {
			cDefs.dumpDrawOrder();
		}

		// Convert raw definitions into processed data
		if (cOptions.convert()) {
			if (cDefs.write(cOptions.outDir())) {
				fprintf(stderr, "Definitions conversion is done!\n");
				exit(0);
			} else {
				fprintf(stderr, "Failed to convert definitions!\n");
				exit(1);
			}
		}

		if (cOptions.plot()) {
			if (cDefs.plotGraph(cOptions.outDir())) {
				fprintf(stderr, "Definitions plot is done!\n");
				exit(0);
			} else {
				fprintf(stderr, "Failed to plot definition graph!\n");
				exit(1);
			}
		}

		// If a number of symbols are given, do not invoke GUI
		if (cOptions.symbolDir()) {
			Recognizer cRecognizer;
			if (cOptions.wrapper()) {
				cRecognizer.wrapper(cOptions.symbolDir());
			} else {
				cRecognizer.recognize(cOptions.symbolDir(), cOptions.match(),
									  cOptions.bestN(), cOptions.assignOrder(),
									  cOptions.verbose(),
									  cOptions.random());
			}
			exit(0);
		}
	}

	int i;
	char buf[4096];
	buf[0]=0;
	for(i=0; i<argc; i++)
	{
		strcat(buf,"\"");
		strcat(buf,argv[i]);
		strcat(buf,"\"");
	}

	HINSTANCE inst;
	inst=(HINSTANCE)GetModuleHandle(NULL);

	// The following lines are added to avoid calling WinMain() directly
	// while linking with FLTK since FLTK implements WinMain() and call main(),
	// which cause infinite loop.
	BOOL AFXAPI AfxInitialize(BOOL bDLL, DWORD dwVersion);
	extern int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
								 LPTSTR lpCmdLine, int nCmdShow);
	AfxInitialize(FALSE, _MFC_VER);
	return AfxWinMain(inst,NULL,buf,SW_SHOWNORMAL);

	//return WinMain(inst,NULL,buf,SW_SHOWMAXIMIZED);
}
